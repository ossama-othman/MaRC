/**
 * @file Mercator.cpp
 *
 * Copyright (C) 1999, 2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "Mercator.h"
#include "Constants.h"
#include "DefaultConfiguration.h"
#include "Mathematics.h"
#include "root_find.h"
#include "OblateSpheroid.h"
// #include "marc/config.h"  // For NDEBUG and FMT_HEADER_ONLY

#ifndef NDEBUG
// # include "Log.h"
#endif

#include <limits>
#include <cmath>

// #include <fmt/core.h>


namespace
{
    /// The underlying Mercator projection equation.
    /**
     * @param[in] body Reference to @c OblateSpheroid object
     *                 representing body being mapped.
     * @param[in] latg Planetographic latitude.
     *
     * @return Value of point on projection along a vertical axis
     *         (e.g. along a longitude line).
     *
     * @note This function is a free function rather than a const
     *       member function to work around buggy implementations of
     *       @c std::bind().
     *
     * @todo Since we no longer use @c std::bind() it should be
     *       possible to once again make this a @c const member
     *       function of @c MaRC::Mercator.
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

MaRC::Mercator::Mercator(std::shared_ptr<OblateSpheroid> body)
    : MapFactory()
    , body_(std::move(body))
{
    using namespace MaRC::default_configuration;

    // The MaRC Mercator projection currently expects a 360 degree
    // longitude range.
    static constexpr int ulps = 2;
    static constexpr double expected_lon_range = 360;

    if (!MaRC::almost_equal(longitude_range, expected_lon_range, ulps))
        throw std::domain_error("Mercator projection requires 360 "
                                "longitude range.");

    // static_assert(default_max_lat < 90,
    //               "Default maximum latitude must be less than
    //               90.");

    // if (!std::isnan(max_lat) && std::abs(max_lat) >= 90) {
    //     throw std::invalid_argument(
    //         fmt::format("Maximum Mercator projection latitude ({}) >= 90.",
    //                     max_lat));
    // }
}

char const *
MaRC::Mercator::projection_name() const
{
    return "Mercator";
}

void
MaRC::Mercator::plot_map(std::size_t samples,
                         std::size_t lines,
                         plot_type plot) const
{
    std::size_t offset = 0;

    /**
     * @todo Confirm that the following calculation is correct.
     */
    /*
      Calculate the largest value of "X" along the axis perpendicular
      to the longitude axis can be plotted on this map, X=0 on the
      equator.  The greater the height of the map, the greater the
      latitude that can be plotted.

       lines    2 * xmax
      ------- = --------
      samples    2 * pi

              lines
      xmax = ------- * pi
             samples
     */
    // No need to take absolute value.  Always positive.
    double const xmax =
        static_cast<double>(lines) / samples * C::pi;

    auto const map_equation =
        [&](double latg){ return mercator_x(*this->body_, latg); };

    for (std::size_t k = 0; k < lines; ++k) {
        double const x = (k + 0.5) / lines * 2 * xmax - xmax;

        /**
         * @todo We shouldn't have to search from pole-to-pole for
         *       the latitude that gives us the above value for @c x.
         *       Try to get the root finding code to work with the
         *       initial guess instead, or at the very least reduce
         *       the size of the search bracket.
         */

        // Obtain initial guess from inverse Mercator equation for a
        // sphere (first eccentricity is zero).
        // double const latg_guess = -C::pi_2 + 2 * std::atan(std::exp(x));

        // double const ll = latg_guess - 0.5;
        // double const ul = latg_guess + 0.5;
        double const ll = -C::pi_2;
        double const ul =  C::pi_2;

        /**
         * @todo Pass in a function that directly computes the first
         *       derivative of the Mercator equation, without relying
         *       on numerical differentation techniques, to speed up
         *       root finding and improve accuracy.
         */
        // planetoGRAPHIC latitude.
        double const latg =
            MaRC::root_find(x, ll, ul, map_equation);

        // MaRC::debug("latg_guess, latg) = ({}, {})", latg_guess, latg);

        // MaRC::debug("(line, latg) = ({}, {})", k, latg / C::degree);

        // Convert to planetoCENTRIC latitude
        double const lat = this->body_->centric_latitude(latg);

        for (std::size_t i = 0; i < samples; ++i, ++offset) {
            double const lon = this->get_longitude(i, samples);

            plot(lat, lon, offset);
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
     * @see MaRC::Mercator::plot_map() to see how this value is
     *      derived.
     */
    // No need to take absolute value.  Always positive.
    double const xmax =
        static_cast<double>(lines) / samples * C::pi;

    double const pix_conv_val = xmax / lines * 2;

    static constexpr auto white =
        std::numeric_limits<typename grid_type::value_type>::max();

    // Draw latitude lines
    for (double n = -90 + lat_interval; n < 90; n += lat_interval) {
        // Convert to planetographic latitude
        double const nn = this->body_->graphic_latitude(n * C::degree);
        double const k =
            std::round(mercator_x(*this->body_, nn) / pix_conv_val
                       + lines / 2.0);

        if (k >= 0 && k < static_cast<double>(lines)) {
            auto const first =
                std::begin(grid)
                + static_cast<decltype(samples)>(k * samples);

            auto const last = first + samples;

            std::fill(first, last, white);
        }
    }

    // Draw longitude lines.
    for (double m = 360; m > 0; m -= lon_interval) {
        decltype(samples) i = std::round(m * samples / 360.0);

        if (this->body_->prograde())
            i = samples - i;

        if (i < samples) {
            for (std::size_t k = 0; k < lines; ++k)
                grid[k * samples + i] = white;
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
     *       expected when it is non-zero (e.g. &pi;).
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
     * @todo A planetographic latitude is required as the argument
     *       which is converted to a planetocentric latitude before
     *       being passed to the @c N() method below, which in turn
     *       converts back to a planetographic latitude before
     *       performing any calculations.  Tweak the method parameters
     *       to avoid the redundant planetographic/centric latitude
     *       conversions.
     */

    // Note that latitude is planetoGRAPHIC.
    return
        this->body_->eq_rad()
        / this->body_->N(this->body_->centric_latitude(latg))
        / std::cos(latg);
}
