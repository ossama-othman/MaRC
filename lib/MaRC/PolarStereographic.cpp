/**
 * @file PolarStereographic.cpp
 *
 * Copyright (C) 2004, 2017-2018  Ossama Othman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 *
 * @author Ossama Othman
 */

#include "MaRC/PolarStereographic.h"
#include "MaRC/OblateSpheroid.h"
#include "MaRC/Constants.h"
#include "MaRC/root_find.h"

#include <functional>
#include <limits>
#include <cmath>
#include <sstream>


namespace
{
    /**
     * @brief Polar Stereographic projection equation coefficient.
     *
     * Calculate the Polar Stereographic projection equation
     * coefficient.  The calculation is done in a separate function
     * rather than directly in the @c MaRC::PolarSteregraphic
     * constructor to simply make the code cleaner.
     *
     * @return Polar Stereographic projection equation coefficient.
     */
    double
    rho_coefficient(MaRC::OblateSpheroid const & body)
    {
        double const a = body.eq_rad();
        double const e = body.first_eccentricity();

        return
            2 * a
            * std::pow(1 + e, (1 - e) / 2)
            * std::pow(1 - e, (1 + e) / 2);
    }

    /**
     * @brief Polar Stereographic projection distortion coefficient.
     *
     * Calculate the Polar Stereographic projection distortion
     * coefficent.  The calculation is done in a separate function
     * rather than directly in the @c MaRC::PolarSteregraphic
     * constructor to simply make the code cleaner.
     *
     * @return Polar Stereographic projection equation coefficient.
     */
    double
    distortion_coefficient(MaRC::OblateSpheroid const & body)
    {
        double const a = body.eq_rad();
        double const e = body.first_eccentricity();

        return
              std::pow(1 + e, 1 - 2 * e)
            * std::pow(1 - e, 1 + 2 * e)
            / (4 * a * a);
    }

    /// The underlying Polar Stereographic projection equation.
    /**
     * @param[in] body  Reference to @c OblateSpheroid object
     *                  representing body being mapped.
     * @param[in] coeff Coefficient used in the Polar
     *                  Stereographic radius 'rho'.
     * @param[in] latg  Bodygraphic latitude.
     *
     * @return Value of point on projection along a radial line
     *         (e.g. along a longitude line).
     *
     * @note This function is a free function rather than a const
     *       member function to work around buggy implementations of
     *       @c std::bind().
     */
    double
    stereo_rho_impl(MaRC::OblateSpheroid const & body,
                    double coeff,
                    double latg)
    {
        double const t = body.first_eccentricity() * std::sin(latg);

        return
            coeff * std::tan(C::pi_4 - latg / 2)
            * std::pow((1 + t) / (1 - t),
                       body.first_eccentricity() / 2);
    }

}

MaRC::PolarStereographic::PolarStereographic(
    std::shared_ptr<OblateSpheroid> body,
    double max_lat,
    bool north_pole)
    : MapFactory()
    , body_(body)
    , max_lat_(std::isnan(max_lat)
               ? 0
               : (north_pole ? max_lat : -max_lat) * C::degree)
    , rho_coeff_(rho_coefficient(*body))
    , distortion_coeff_(distortion_coefficient(*body))
    , north_pole_(north_pole)
{
    if (!std::isnan(max_lat) && std::abs(max_lat) >= 90) {
        std::ostringstream s;
        s << "Maximum Polar Stereographic projection latitude ("
          << max_lat << ") >= 90.";

        throw std::invalid_argument(s.str());
    }
}

const char *
MaRC::PolarStereographic::projection_name() const
{
    return "Polar Stereographic";
}

void
MaRC::PolarStereographic::plot_map(std::size_t samples,
                                   std::size_t lines,
                                   plot_type plot) const
{
    std::size_t const nelem = samples * lines;

    std::size_t offset = 0;

    /*
      The maximum "rho" at the smaller of the map dimensions.  For
      example, given a map with 50 samples and 25 lines, the maximum
      "rho" will be at the lower edge of line 1 and the upper edge of
      line 25.
    */
    double const rho_max =
        this->stereo_rho(this->body_->graphic_latitude(this->max_lat_));
    auto const min_dim = std::min(samples, lines);
    double const pix_conv_val = 2 * rho_max / min_dim;

    /**
     * @todo Confirm that the use of the term "counter-clockwise" is
     *       correct.  It looks like "clockwise" is what we're
     *       expecting.
     */
    // Whether longitudes increase counter-clockwise in map.
    bool const ccw =
        ((this->north_pole_ && this->body_->prograde())
         || (!this->north_pole_ && !this->body_->prograde()));

    using namespace std::placeholders;
    auto const map_equation =
        std::bind(stereo_rho_impl,
                  std::cref(*this->body_),
                  this->rho_coeff_,
                  _1);

    for (std::size_t k = 0; k < lines; ++k) {
        double const X = k + 0.5 - lines / 2.0;

        for (std::size_t i = 0; i < samples; ++i, ++offset) {
            double const Y   = i + 0.5 - samples / 2.0;

            /**
             * @note Rho may actually be larger than rho_max when
             *       mapping pixels along the larger of the map
             *       dimensions.  That should be okay since rho will
             *       never correspond to the pole that isn't at the
             *       center of the map.
             */
            double const rho = pix_conv_val * std::hypot(Y, X);

            /*
              Obtain an initial guess by solving the polar
              stereographic projection equation for the latitude for a
              spherical body (first eccentricity is zero).
            */
            double const latg_guess =
                C::pi_2 - 2 * std::atan(rho / 2 / this->body_->eq_rad());

            double const ll = -C::pi_2;
            double const ul =  C::pi_2;

            /**
             * @todo Pass in a function that directly computes the
             *       first derivative of the Polar Stereographic
             *       equation, without relying on numerical
             *       differentation techniques, to speed up root
             *       finding and improve accuracy.
             */

            // bodyGRAPHIC latitude.
            double const latg =
                MaRC::root_find(rho, ll, ul, map_equation);

            std::cout << "*** (latg_guess, latg) = ("
                      << latg_guess << ", "
                      << latg << ")\n";

            // Convert to bodyCENTRIC latitude
            double const lat =
                this->body_->centric_latitude(this->north_pole_
                                              ? latg : -latg);

            double const lon =
                std::atan2((ccw ? Y : -Y), X);

            unsigned char const percent_complete =
                static_cast<unsigned char>((offset + 1) * 100 / nelem);

            plot(lat, lon, percent_complete, offset);
        }
    }
}

void
MaRC::PolarStereographic::plot_grid(std::size_t samples,
                                    std::size_t lines,
                                    double lat_interval,
                                    double lon_interval,
                                    grid_type & grid) const
{
    static constexpr std::size_t imax = 2000;

    double const rho_max =
        this->stereo_rho(this->body_->graphic_latitude(this->max_lat_));
    double const min_dim = std::min(samples - 1, lines - 1);
    double const pix_conv_val = 2 * rho_max / min_dim;

    static constexpr auto white =
        std::numeric_limits<typename grid_type::value_type>::max();

    // Draw latitude lines
    for (double n = -90 + lat_interval; n < 90; n += lat_interval) {
        /**
         * @bug Shouldn't we take into account the pole at the center
         *      and maximum latitude of the projection here?
         */

        // Convert to bodygraphic latitude
        double const nn = this->body_->graphic_latitude(n * C::degree);

        double const rho = this->stereo_rho(nn);

        for (std::size_t m = 0; m < imax; ++m) {
            double const mm =
                static_cast<double>(m) / imax * C::degree * 360;

            double const z = rho * std::cos(mm);
            double const x = rho * std::sin(mm);

//           if (z > rho_max || x > rho_max)
//             continue;

            double const k = std::round(z / pix_conv_val + lines / 2.0);
            double const i = std::round(x / pix_conv_val + samples / 2.0);

            if (i >= 0 && i < static_cast<double>(samples)
                && k >= 0 && k < static_cast<double>(lines)) {
                std::size_t const offset =
                    static_cast<std::size_t>(k) * samples;

                grid[offset + static_cast<std::size_t>(i)] = white;
            }
        }
    }

    // Draw longitude lines.
    for (double m = 360; m > 0; m -= lon_interval) {
        double const mm = m * C::degree;  // Convert to radians

        for (std::size_t n = 0; n < imax; ++n) {
            double const nn =
                static_cast<double>(n) / imax * C::degree * 360;

            /**
             * @bug Shouldn't we take into account the pole at the
             *      center and maximum latitude of the projection
             *      here?
             */

            double const rho = this->stereo_rho(nn);

            double const z = rho * std::cos(mm);
            double const x = rho * std::sin(mm);

//           if (z > rho_max || x > rho_max)
//             continue;

            double const k = std::round(z / pix_conv_val + lines / 2.0);
            double const i = std::round(x / pix_conv_val + samples / 2.0);

            if (i >= 0 && i < static_cast<double>(samples)
                && k >= 0 && k < static_cast<double>(lines)) {
                std::size_t const offset =
                    static_cast<std::size_t>(k) * samples;

                grid[offset + static_cast<std::size_t>(i)] = white;
            }
        }
    }
}

double
MaRC::PolarStereographic::distortion(double latg) const
{
    // Note that latitude is bodyGRAPHIC.
    return 1 + distortion_coeff_ * std::pow(this->stereo_rho(latg), 2);
}

double
MaRC::PolarStereographic::stereo_rho(double latg) const
{
    return stereo_rho_impl(*this->body_, this->rho_coeff_, latg);
}
