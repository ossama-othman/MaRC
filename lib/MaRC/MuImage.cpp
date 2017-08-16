/**
 * @file MuImage.cpp
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
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

#include "MuImage.h"
#include "OblateSpheroid.h"
#include "Constants.h"

#include <cmath>


MaRC::MuImage::MuImage(std::shared_ptr<OblateSpheroid> body,
                       double sub_observ_lat,
                       double sub_observ_lon,
                       double range,
                       double scale,
                       double offset)
    : VirtualImage(scale, offset)
    , body_(body)
    , sub_observ_lat_(sub_observ_lat * C::degree) // Radians
    , sub_observ_lon_(sub_observ_lon * C::degree) // Radians
    , range_(range)
{
}

bool
MaRC::MuImage::read_data_i(double lat, double lon, double & data) const
{
    data = this->body_->mu(this->sub_observ_lat_,
                           this->sub_observ_lon_,
                           lat,
                           lon,
                           this->range_);

    return true;
}

bool
MaRC::MuImage::is_visible_i(OblateSpheroid const & body,
                            double sub_observ_lat,
                            double sub_observ_lon,
                            double lat,
                            double lon,
                            double range)
{
    /**
     * @todo This is ugly.  The visibility check is specific to an
     *       oblate spheroid.  It should really be moved to the
     *       @c OblateSpheroid strategy.  Alternatively, is there any
     *       reason why we can't check if this->body_->mu() < 0 to
     *       determine if the given latitude and longitude isn't
     *       visible?  That would allow us to drop the OblateSpheroid
     *       dependency entirely.
     */

    double const latg   = body.graphic_latitude(lat);

    double const radius = body.centric_radius(lat);

    double const cosine =
        (radius * std::cos(lat - latg)
         - range * std::sin(sub_observ_lat) * std::sin (latg))
        / range / std::cos(sub_observ_lat) / std::cos(latg);

    if (cosine >= -1 && cosine <= 1) {
        // Partial range of longitudes are visible

        double const lower = sub_observ_lon - std::abs(std::acos(cosine));
        double const upper = sub_observ_lon + std::abs(std::acos(cosine));

        // Now check if longitude at given latitude is visible
        double l = lon;

        if (l < lower)
            l += C::_2pi;
        else if (l > upper)
            l -= C::_2pi;

        if (l >= lower && l <= upper)
            return true;
    } else if (cosine < -1) {
         // Full 360 degree visible longitude range
        return true;
    }

  return false;
}

bool
MaRC::MuImage::is_visible(double lat, double lon) const
{
    return MaRC::MuImage::is_visible_i(*this->body_,
                                       this->sub_observ_lat_,
                                       this->sub_observ_lon_,
                                       lat,
                                       lon,
                                       this->range_);
}
