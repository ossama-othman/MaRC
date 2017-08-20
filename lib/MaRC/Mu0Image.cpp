/**
 * @file Mu0Image.cpp
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

#include "Mu0Image.h"
#include "OblateSpheroid.h"
#include "Constants.h"

#include <cmath>


MaRC::Mu0Image::Mu0Image(std::shared_ptr<OblateSpheroid> body,
                         double sub_solar_lat,
                         double sub_solar_lon,
                         double scale,
                         double offset)
    : VirtualImage(scale, offset)
    , body_(body)
    , sub_solar_lat_(sub_solar_lat * C::degree)  // Radians
    , sub_solar_lon_(sub_solar_lon * C::degree)  // Radians
{
}

bool
MaRC::Mu0Image::read_data_i(double lat, double lon, double & data) const
{
    data = this->body_->mu0(this->sub_solar_lat_,
                            this->sub_solar_lon_,
                            lat,
                            lon);

  return true;
}

bool
MaRC::Mu0Image::is_visible(double lat, double lon) const
{
    /**
     * @todo This is ugly.  The visibility check is specific to an
     *       oblate spheroid.  It should really be moved to
     *       OblateSpheroid strategy.  Alternatively, is there any
     *       reason why we can't check if this->body_->mu0() < 0 to
     *       determine if the given latitude and longitude isn't
     *       visible?  That would allow us to drop the OblateSpheroid
     *       dependency entirely.
     */
    double const latg = this->body_->graphic_latitude(lat);

    // When creating cosine of incidence angle map use terminator
    // instead of limb so that entire incidence angle map is created
    // as opposed to just the area visible to the observer.
    double const cosine = std::tan(latg) * std::tan(this->sub_solar_lat_);

    if (cosine >= -1 && cosine <= 1) {
        // Partial range of longitudes are visible
      double const lower = this->sub_solar_lon_ - std::abs(std::acos(-cosine));
      double const upper = this->sub_solar_lon_ + std::abs(std::acos(-cosine));

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
