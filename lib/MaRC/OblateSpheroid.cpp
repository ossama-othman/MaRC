/**
 * @file OblateSpheroid.cpp
 *
 * Copyright (C) 1999, 2003-2004, 2017  Ossama Othman
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

#include "OblateSpheroid.h"
#include "Vector.h"
#include "Mathematics.h"

#include <stdexcept>
#include <cmath>
#include <limits>
#include <sstream>
#include <cassert>


MaRC::OblateSpheroid::OblateSpheroid(bool prograde,
                                     double eq_rad,
                                     double pol_rad,
                                     double flattening)
    : BodyData(prograde)
    , eq_rad_(-1)
    , pol_rad_(-1)
    , flattening_(-1)
    , first_eccentricity_(-1)
{
    this->initialize_radii(eq_rad, pol_rad, flattening);
}

MaRC::OblateSpheroid::~OblateSpheroid()
{
}

void
MaRC::OblateSpheroid::initialize_radii(double eq_rad,
                                       double pol_rad,
                                       double flattening)
{
    unsigned int count = 0;

    // ----------------------------------------------
    // Set equatorial radius.
    // ----------------------------------------------
    if (eq_rad > 0
        && eq_rad < std::sqrt(std::numeric_limits<double>::max()) - 1) {
        if (eq_rad < this->pol_rad_) {
            std::ostringstream s;
            s << "Equatorial radius (" << eq_rad
              << ") is less than polar radius (" << this->pol_rad_ << ")";

            throw std::invalid_argument(s.str());
        } else {
            this->eq_rad_ = eq_rad;

            ++count;
        }
    }

    // ----------------------------------------------
    // Set polar radius.
    // ----------------------------------------------
    if (pol_rad > 0
        && pol_rad < std::sqrt(std::numeric_limits<double>::max()) - 1) {
        if (this->eq_rad_ > 0 && this->eq_rad_ < pol_rad) {
            std::ostringstream s;
            s << "Polar radius (" << pol_rad
              << ") is greater than equatorial radius ("
              << this->eq_rad_ << ")";

            throw std::invalid_argument(s.str());
        } else {
            this->pol_rad_ = pol_rad;

            ++count;
        }
    }

    // ----------------------------------------------
    // Set flattening.
    // ----------------------------------------------

    // flattening <  0 ----> Prolate Spheroid <----- Not valid
    // flattening > 0 < 1 -> Oblate Spheroid
    // flattening == 0 ----> Sphere
    // flattening == 1 ----> Disc!!  <--- Not valid

    if (flattening >= 0 && flattening < 1) {
        this->flattening_ = flattening;
        this->first_eccentricity_ =
            std::sqrt(1 - std::pow(1 - flattening, 2));

        ++count;
    }

    if (count < 2) {
        std::ostringstream s;
        s << "< " << count << " > valid oblate spheroid "
          << "characteristic(s) specified:"
          << "\n  Equatorial radius: " << eq_rad
          << "\n  Polar radius:      " << pol_rad
          << "\n  Flattening:        " << flattening
          << "\nTwo are required.";

        throw std::invalid_argument(s.str());
    }

    // At least two components have been set.  Set the third.

    if (this->eq_rad_ < 0)
        this->eq_rad_ = this->pol_rad_ / (1 - this->flattening_);
    else if (this->pol_rad_ < 0)
        this->pol_rad_ = this->eq_rad_ * (1 - this->flattening_);
    else if (this->flattening_ < 0) {
        this->flattening_ = 1 - this->pol_rad_ / this->eq_rad_;
        this->first_eccentricity_ =
            std::sqrt(1 - std::pow(1 - this->flattening_, 2));
    }
}

double
MaRC::OblateSpheroid::centric_radius(double lat) const
{
    /*
      Given a bodycentric latitude and longitude for a point (x, y, z)
      on the surface of a spheroid:

          x = r * cos(lat) * cos(lon)
          y = r * cos(lat) * sin(lon)
          z = r * sin(lat)

      Assume longitude zero is along the observer optical axis, we
      have:

          x = r * cos(lat)
          y = 0
          z = r * sin(lat)

      The Cartesian equation for an oblate spheroid is:

           2    2    2
          x  + y    z
          ------- + -- = 1
             2       2
            a       c

      We end up with:

                       2                  2
         (r * cos(lat))  +  (r * sin(lat))
         ---------------    --------------- = 1
                 2                   2
                a                   c

      and:

                            1
        r = -----------------------------------
                             2               2
            sqrt((cos(lat)/a)  + (sin(lat)/c) )

    */
    return 1 / std::hypot(std::cos(lat) / this->eq_rad_,
                          std::sin(lat) / this->pol_rad_);
}

double
MaRC::OblateSpheroid::centric_latitude(double latg) const
{
    /*
                                  2
                    (polar radius)
      tan(lat) = -------------------- * tan(latg)
                                    2
                 (equatorial radius)
    */
    return
        std::atan(std::pow(this->pol_rad_ / this->eq_rad_, 2)
                  * std::tan(latg));
}

double
MaRC::OblateSpheroid::graphic_latitude(double lat) const
{
    /*
                                     2
                  (equatorial radius)
      tan(latg) = -------------------- * tan(lat)
                                   2
                     (polar radius)
    */
    return
        std::atan(std::pow(this->eq_rad_ / this->pol_rad_, 2)
                  * std::tan(lat));
}

double
MaRC::OblateSpheroid::mu(double sub_observ_lat,
                         double sub_observ_lon,
                         double lat,
                         double lon,
                         double range) const
{
    // Compute mu, the cosine of local normal-observer (emission)
    // angle.

    double const latg = this->graphic_latitude(lat);
    double const ellipse_radius = this->centric_radius(lat);

    return
        ((range * std::sin(sub_observ_lat) * std::sin(latg)
          - ellipse_radius * std::cos(lat - latg))
         + range * std::cos(sub_observ_lat) *
         std::cos(latg) * std::cos(sub_observ_lon - lon)) /
        // Dot product (above) divided by the magnitude of vector from
        // observer (e.g. spacecraft camera) to point on body.
        std::sqrt(range * range + ellipse_radius * ellipse_radius -
                  2 * range * ellipse_radius *
                  (std::sin(sub_observ_lat) * std::sin(lat) +
                   std::cos(sub_observ_lat) *
                   std::cos(lat) * std::cos(sub_observ_lon - lon)));
}

double
MaRC::OblateSpheroid::mu0(double sub_solar_lat,
                          double sub_solar_lon,
                          double lat,
                          double lon) const
{
    // Compute mu0, the cosine of sun-local normal (incidence) angle.

    double const latg = this->graphic_latitude(lat);

    return
        std::sin(sub_solar_lat) * std::sin(latg)
        + std::cos(sub_solar_lat) * std::cos(latg)
          * std::cos(sub_solar_lon - lon);
    // The above equation assumes the sun to be an infinite distance
    // away.
}

// Cosine of phase angle (observer range taken into account)
double
MaRC::OblateSpheroid::cos_phase(double sub_observ_lat,
                                double sub_observ_lon,
                                double sub_solar_lat,
                                double sub_solar_lon,
                                double lat,
                                double lon,
                                double range) const
{
    // Compute the cosine of the Sun-point on surface of body-Observer
    // angle, i.e cosine of the phase angle Phi.

    double const ellipse_radius = this->centric_radius(lat);

    return
        (range * (std::cos(sub_observ_lat) * std::cos(sub_solar_lat) *
                  std::cos(sub_observ_lon - sub_solar_lon) +
                  std::sin(sub_observ_lat) * std::sin(sub_solar_lat)) -
         ellipse_radius * (std::cos(lat) * std::cos (sub_solar_lat) *
                           std::cos(lon - sub_solar_lon) +
                           std::sin(lat) * std::sin(sub_solar_lat))) /
        // Dot product (above) divided by the magnitude of vector from
        // observer to point on body, defining unit vector for vector
        // to sun (infinite distance away).
        std::sqrt(range * range + ellipse_radius * ellipse_radius
                  - 2 * range * ellipse_radius *
                  (std::sin(sub_observ_lat) * std::sin(lat)
                   + std::cos(sub_observ_lat) *
                   std::cos(lat) * std::cos(sub_observ_lon - lon)));
}

double
MaRC::OblateSpheroid::M(double lat)
{
    double const fe2 =
        this->first_eccentricity_ * this->first_eccentricity_;
    double const sin_latg = std::sin(this->graphic_latitude(lat));

  return
    this->eq_rad_ * (1 - fe2)
      / std::pow(1 - fe2 * sin_latg * sin_latg, 1.5);
}

double
MaRC::OblateSpheroid::N(double lat)
{
    double const sin_latg = std::sin(this->graphic_latitude(lat));

    return
        this->eq_rad_
        / std::sqrt(1 - this->first_eccentricity_
                        * this->first_eccentricity_
                        * sin_latg * sin_latg);
}

int
MaRC::OblateSpheroid::ellipse_intersection(DVector const & vec,
                                           DVector const & dvec,
                                           double & lat,
                                           double & lon) const
{
    /*
      intersection of tri-axial ellipsoid with line
      - Ellipsoid: 1 = x^2/(semis[0]^2) + y^2/(semis[1]^2) + z^2/(semis[2]^2)
      - Line: vec> + k * dvec>
      - vec> and dvec> are vectors;
      - k is a scalar representing distance from vec if dvec>
        is a vector that crosses the surface of the ellipsoid
        at least once.
      - ** returns lat and lon based on solution closest to vec> first
          (i.e. k.second < k.first)

      output:

      - lat --  equivalent to a planetocentric latitude
      - lon --  equivalent to a planetocentric longitude

      output_ (intermediate):

      - std::pair<> k  scalar k in (vec> + k * dvec>)
                                - up to 2 solutions possible
      - solutions returned: -1 (bad inputs), 1 (no solution) or 0 (successful)
    */

    // Semi-major axis along x-axis
    double const & Axis_a = this->eq_rad_;

    // Semi-major axis along y-axis
    double const & Axis_b = this->eq_rad_;

    // Semi-major axis along z-axis
    double const & Axis_c = this->pol_rad_;

    double const semis[] = { Axis_a, Axis_b, Axis_c };

    // Convert inputs to quadratic coefficients
    double a = 0;
    double b = 0;
    double c = -1;

    for (std::size_t i = 0; i < 3; ++i) {
        double const m1 = dvec[i] / semis[i];
        double const m2 =  vec[i] / semis[i];
        a +=     m1 * m1;
        b += 2 * m1 * m2;
        c +=     m2 * m2;
    }

    // Check for bad inputs (a=0 => dvec> is null)

    if (a <= 0)
        return -1; // Unsuccessful

    // Check for no solution; return closest solution in k0 if no
    // solution

    std::pair<double, double> k;

    if (!MaRC::quadratic_roots(a, b, c, k)) {
        // Not on ellipsoid ... no solution
        return 1;  // Unsuccessful
    }

    // std::cout << "k0 = " << k.first << "  \tk1 = " << k.second << '\n';

    //   if (k.first < k.second)  // This is unlikely to be true.
    //   //  if (std::abs(k.first) < std::abs(k.second))
    //     k.second = k.first;

    // Use k.second as solution

    assert(k.second < k.first);

    double const x = vec[0] + k.second * dvec[0];
    double const y = vec[1] + k.second * dvec[1];
    double const z = vec[2] + k.second * dvec[2];

    lat = std::atan(z / std::hypot(x, y));
    lon = std::atan2(x, -y);

    /*
      lon is in EAST LONGITUDE.  Zero longitude is on negative y-axis!
      y is increasing moving away from observer.
    */

    return 0; // Successful
}
