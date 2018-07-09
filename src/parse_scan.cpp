/**
 * @file parse_scan.cpp
 *
 * Copyright (C) 1999, 2004, 2017  Ossama Othman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * @author Ossama Othman
 */

#include "parse_scan.h"
#include "MapCommand.h"

#include <stdexcept>
#include <sstream>
#include <limits>


MaRC::ParseParameter::ParseParameter()
    : filename(nullptr)
    , lat_interval(10)
    , lon_interval(10)
    , minimum(std::numeric_limits<decltype(this->minimum)>::lowest())
    , maximum(std::numeric_limits<decltype(this->maximum)>::max())
    , nibble_left  (0)
    , nibble_right (0)
    , nibble_top   (0)
    , nibble_bottom(0)
    , commands_()
    , sym_table_()
{
}

void
MaRC::ParseParameter::push_command(std::unique_ptr<MapCommand> c)
{
    this->commands_.push_back(std::move(c));
}

// -------------------------------------------------------------------

void
MaRC::Radii::validate()
{
    int count = 0;

    if (this->eq_rad > 0)
            ++count;

    if (this->pol_rad > 0)
            ++count;

    /*
      flattening = (eq_rad - pol_rad) / eq_rad

          flattening == 0 : Sphere
      0 < flattening <  1 : Oblate Spheroid

          flattening <  0 : eq_rad < pol_rad : INVALID (prolate spheroid)
      1 < flattening      : pol_rad < 0      : INVALID
          flattening == 1 : Disc             : INVALID
    */
    if (this->flattening >= 0 && this->flattening < 1)
        ++count;

    if (count < 2) {
        std::ostringstream s;
        s << "< " << count << " > valid oblate spheroid "
          << "characteristic(s) specified:"
          << "\n  Equatorial radius: " << this->eq_rad
          << "\n  Polar radius:      " << this->pol_rad
          << "\n  Flattening:        " << this->flattening
          << "\nTwo are required.";

        throw std::invalid_argument(s.str());
    }

    // At least two components have been set.  Set the third.
    if (this->eq_rad < 0)
        this->eq_rad = this->pol_rad / (1 - this->flattening);
    else if (this->pol_rad < 0)
        this->pol_rad = this->eq_rad * (1 - this->flattening);

    // MaRC currently only support oblate spheroids.
    if (this->eq_rad < this->pol_rad) {
        std::ostringstream s;
        s << "Equatorial radius (" << this->eq_rad
          << ") is less than polar radius (" << this->pol_rad << ")";

        throw std::invalid_argument(s.str());
    }
}

// -------------------------------------------------------------------

