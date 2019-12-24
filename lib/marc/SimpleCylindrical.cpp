/**
 * @file SimpleCylindrical.cpp
 *
 * Copyright (C) 1996-1997, 1999, 2003-2004, 2017-2018  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#include "SimpleCylindrical.h"
#include "Constants.h"
#include "Validate.h"
#include "Mathematics.h"
#include "Log.h"

#include <limits>
#include <cmath>
#include <algorithm>


namespace
{
    /**
     * @brief Get Simple Cylindrical map boundary latitude.
     *
     * Place supplied map boundary latitude in a form suitable for use
     * by the %MaRC Simple Cylindrical projection.  That entails
     * converting it to radians, and potentially converting it to a
     * planetographic latitude.
     *
     * This function is meant to be used during map initialization,
     * and exists predominantly to allow for initialization of
     * @c const map boundary latitude members.
     *
     * @param[in] degrees        Planetocentric latitude in degrees.
     * @param[in] body           The body being mapped.
     * @param[in] planetographic Whether or not latitude in the map
     *                           will be planetographic.
     *
     * @return Latitude in radians.  The returned latitude will be
     *         planetographic if the @a planetographic argument is
     *         @c true.
     */
    double boundary_latitude(double degrees,
                             MaRC::BodyData const * body,
                             bool planetographic)
    {
        double latitude = MaRC::validate_latitude(degrees);

        // All latitudes fed to SimpleCylindrical are
        // planetoCENTRIC. Convert to planetoGRAPHIC latitude if
        // requested.
        if (planetographic)
            latitude = body->graphic_latitude(latitude);

        return latitude;
    }
}

// -------------------------------------------------------------------

MaRC::SimpleCylindrical::SimpleCylindrical(
    std::shared_ptr<BodyData> body,
    double lo_lat,
    double hi_lat,
    double lo_lon,
    double hi_lon,
    bool graphic_lat)
    : MapFactory()
    , body_(std::move(body))
    , lo_lat_(boundary_latitude(lo_lat, body_.get(), graphic_lat))
    , hi_lat_(boundary_latitude(hi_lat, body_.get(), graphic_lat))
    , lo_lon_(MaRC::validate_longitude(lo_lon))
    , hi_lon_(MaRC::validate_longitude(hi_lon))
    , graphic_lat_(graphic_lat)
{
    // Set lower longitude to equivalent longitude less than upper
    // longitude or add 360 degrees to the upper longitude if it is
    // equal to the lower longitude (i.e. full 360 degree range) to
    // make sure longitude range is computed correctly.
    constexpr int ulps = 2;
    if (this->lo_lon_ > this->hi_lon_)
        this->lo_lon_ -= C::_2pi;
    else if (MaRC::almost_equal(this->lo_lon_, this->hi_lon_, ulps)
             || (MaRC::almost_zero(this->lo_lon_, ulps)
                 && MaRC::almost_zero(this->hi_lon_, ulps))) {
        this->hi_lon_ += C::_2pi;

        MaRC::info("lower and upper map longitudes are the same");
        MaRC::info("assuming 360 degree longitude range");
    }
}

char const *
MaRC::SimpleCylindrical::projection_name() const
{
    return "Simple Cylindrical";
}

void
MaRC::SimpleCylindrical::plot_map(std::size_t samples,
                                  std::size_t lines,
                                  plot_type plot) const
{
    // Conversion factor -- latitudes per line
    double const cf = (this->hi_lat_ - this->lo_lat_) / lines;

    std::size_t offset = 0;

    for (std::size_t k = 0; k < lines; ++k) {
        // Compute latitude at center of pixel.
        double lat = (k + 0.5) * cf + this->lo_lat_;

        // Convert to CENTRIC Latitude
        if (this->graphic_lat_)
            lat = this->body_->centric_latitude(lat);

        for (std::size_t i = 0; i < samples; ++i, ++offset) {
            double const lon = this->get_longitude(i, samples);

            plot(lat, lon, offset);
        }
    }
}

void
MaRC::SimpleCylindrical::plot_grid(std::size_t samples,
                                   std::size_t lines,
                                   double lat_interval,
                                   double lon_interval,
                                   grid_type & grid) const
{
    // Convert back to degrees
    double const lo_lat = this->lo_lat_ / C::degree;
    double const hi_lat = this->hi_lat_ / C::degree;
    double const lo_lon = this->lo_lon_ / C::degree;
    double const hi_lon = this->hi_lon_ / C::degree;

    // Line-to-latitude ratio.
    double const lr = lines / (hi_lat - lo_lat);

    static constexpr auto white =
        std::numeric_limits<typename grid_type::value_type>::max();

    // Draw latitude lines
    for (double n = -90 + lat_interval; n < 90; n += lat_interval) {
        double const k = std::round((n - lo_lat) * lr);

        if (k >= 0 && k < static_cast<double>(lines)) {
            auto const first =
                std::begin(grid)
                + static_cast<decltype(samples)>(k * samples);

            auto const last = first + samples;

            std::fill(first, last, white);
        }
    }

    // Sample-to-longitude ratio.
    double const sr = samples / (hi_lon - lo_lon);

    /**
     * @todo Why do we count down grid longitudes from 360 instead of
     *       counting up from 0?  As is, we are plotting longitude
     *       lines in the half-open interval (0, 360] instead of
     *       [0, 360).  Aren't they equivalent?
     */
    // Draw longitude lines.
    for (double m = 360; m > 0; m -= lon_interval) {
        // lo_lon_2 is a work-around for lo_lon_ > hi_lon_ problems
        double lo_lon_2;

        if (m - lo_lon > 360)
            lo_lon_2 = lo_lon + 360;
        else
            lo_lon_2 = lo_lon;

        decltype(samples) i = std::round((m - lo_lon_2) * sr);

        if (this->body_->prograde())
            i = samples - i;

        if (i < samples) {
            for (std::size_t k = 0; k < lines; ++k)
                grid[k * samples + i] = white;
        }
    }
}
