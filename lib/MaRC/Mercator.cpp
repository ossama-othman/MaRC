/**
 * @file Mercator.cpp
 *
 * Copyright (C) 1999, 2004, 2017-2018  Ossama Othman
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

#include "Mercator.h"
#include "Constants.h"
#include "DefaultConfiguration.h"
#include "Mathematics.h"
#include "root_find.h"
#include "OblateSpheroid.h"

#include <functional>
#include <limits>
#include <cmath>
#include <sstream>


namespace
{
    /// The underlying Transverse Mercator projection equation.
    /**
     * @param[in] body Reference to @c OblateSpheroid object
     *                 representing body being mapped.
     * @param[in] latg Bodygraphic latitude.
     *
     * @return Value of point on projection along a vertical axis
     *         (e.g. along a longitude line).
     *
     * @note This function is a free function rather than a const
     *       member function to work around buggy implementations of
     *       @c std::bind().
     */
    double
    mercator_x(MaRC::OblateSpheroid const & body, double latg)
    {
        double const t = body.first_eccentricity() * std::sin(latg);

        return
            std::log(std::tan(C::pi_4 + latg / 2)
                     * std::pow((1 - t) / (1 + t),
                                body.first_eccentricity() / 2));
    }
}

MaRC::Mercator::Mercator(std::shared_ptr<OblateSpheroid> body,
                         double max_lat)
    : MapFactory()
    , body_(body)
    , lat_range_((std::isnan(max_lat)
                  ? default_max_lat
                  : max_lat) * C::degree * 2)
{
    using namespace MaRC::default_configuration;

    // The MaRC Mercator projection currently expects a 360 degree
    // longitude range.
    static constexpr int ulps = 2;
    static constexpr double expected_lon_range = 360;

    if (!MaRC::almost_equal(longitude_range, expected_lon_range, ulps))
        throw std::domain_error("Mercator projection requires 360 "
                                "longitude range.");

    static_assert(default_max_lat < 90,
                  "Default maximum latitude must be less than 90.");

    if (!std::isnan(max_lat) && std::abs(max_lat) >= 90) {
        std::ostringstream s;
        s << "Maximum Mercator projection latitude ("
          << max_lat << ") >= 90.";

        throw std::invalid_argument(s.str());
    }
}

char const *
MaRC::Mercator::projection_name() const
{
    return "Transverse Mercator";
}

void
MaRC::Mercator::plot_map(std::size_t samples,
                         std::size_t lines,
                         plot_type plot) const
{
    std::size_t const nelem = samples * lines;

    std::size_t offset = 0;

    /**
     * @bug This calculation doesn't appear to be correct.  Shouldn't
     *      twice the @c mercator_x(*this->body_, max_lat) be used to
     *      determine the @c xmax value?
     *
     * @see MaRC::PolarStereographic::plot_map()
     */
    // No need to take absolute value.  Always positive.
    double const xmax =
        static_cast<double>(lines) / samples * this->lat_range_;

    using namespace std::placeholders;
    auto const map_equation =
        std::bind(mercator_x, std::cref(*this->body_), _1);

    for (std::size_t k = 0; k < lines; ++k) {
        double const x = (k + 0.5) / lines * 2 * xmax - xmax;

        // Obtain initial guess from inverse Mercator equation for a
        // sphere.
        double const latg_guess = -C::pi_2 + 2 * std::atan(std::exp(x));

        /**
         * @todo Pass in a function that directly computes the first
         *       derivative of the Mercator equation, without relying
         *       on numerical differentation techniques, to speed up
         *       root finding and improve accuracy.
         */
        double const latg =
            MaRC::root_find(
                x,
                latg_guess,   // bodyGRAPHIC latitude
                map_equation);

        // Convert to bodyCENTRIC latitude
        double const lat = this->body_->centric_latitude(latg);

        for (std::size_t i = 0; i < samples; ++i, ++offset) {
            double const lon = this->get_longitude(i, samples);

            unsigned char const percent_complete =
                static_cast<unsigned char>((offset + 1) * 100 / nelem);

            plot(lat, lon, percent_complete, offset);
        }
    }
}

void
MaRC::Mercator::plot_grid(std::size_t samples,
                          std::size_t lines,
                          double lat_interval,
                          double lon_interval,
                          grid_type & grid) const
{
    /**
     * @bug This calculation doesn't appear to be correct.  Shouldn't
     *      twice the @c mercator_x(*this->body_, max_lat) be used to
     *      determine the @c xmax value?
     *
     * @see MaRC::PolarStereographic::plot_grid()
     */
    // No need to take absolute value.  Always positive.
    double const xmax =
        static_cast<double>(lines) / samples * this->lat_range_;

    double const pix_conv_val = xmax / lines * 2;

    static constexpr auto white =
        std::numeric_limits<typename grid_type::value_type>::max();

    // Draw latitude lines
    for (double n = -90 + lat_interval; n < 90; n += lat_interval) {
        // Convert to bodygraphic latitude
        double const nn = this->body_->graphic_latitude(n * C::degree);

        /**
         * @bug Shouldn't we take into account the maximum latitude of
         *      the projection here?
         */

        double const k =
            std::round(mercator_x(*this->body_, nn) / pix_conv_val
                       + lines / 2.0);

        if (k >= 0 && k < static_cast<double>(lines)) {
            auto const first =
                std::begin(grid)
                + static_cast<std::size_t>(k) * samples;

            auto const last = first + samples;

            std::fill(first, last, white);
        }
    }

    // Draw longitude lines.
    for (double m = 360; m > 0; m -= lon_interval) {
        int i = static_cast<int>(std::round(m * samples / 360.0));

        if (this->body_->prograde())
            i = samples - i;

        if (i >= 0 && static_cast<std::size_t>(i) < samples) {
            for (std::size_t k = 0; k < lines; ++k)
                grid[k * lines + static_cast<std::size_t>(i)] = white;
        }
    }
}

double
MaRC::Mercator::get_longitude(std::size_t i, std::size_t samples) const
{
    using namespace MaRC::default_configuration;

    static constexpr double lo_lon = longitude_low * C::degree;

    /**
     * @todo Verify that the shift by @c lo_lon below works as
     *       expected when it is non-zero (e.g. -C::pi).
     */

    // Compute longitude at center of pixel.
    double lon = (i + 0.5) / samples * C::_2pi + lo_lon;

    // PROGRADE ----> longitudes increase to the left
    // RETROGRADE --> longitudes increase to the right

    if (this->body_->prograde())
        lon = C::_2pi - lon;

    return lon;
}

double
MaRC::Mercator::distortion(double latg) const
{
    /**
     * @todo A graphic latitude is required as the argument which is
     *       converted to a centric latitude before being passed to
     *       the @c N() method below, which in turn converts back to a
     *       graphic latitude before performing any calculations.
     *       Tweak the method parameters to avoid the redundant
     *       graphic/centric latitude conversions.
     */

    // Note that latitude is bodyGRAPHIC.
    return
        this->body_->eq_rad()
        / this->body_->N(this->body_->centric_latitude(latg))
        / std::cos(latg);
}
