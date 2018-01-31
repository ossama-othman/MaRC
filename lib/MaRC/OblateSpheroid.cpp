/**
 * @file OblateSpheroid.cpp
 *
 * Copyright (C) 1999, 2003-2004, 2017-2018  Ossama Othman
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


namespace
{
    double validate_radius(double r)
    {
        if (std::isnan(r) || r <= 0)
            throw std::invalid_argument(
                "Invalid oblate spheroid radius.");

        return r;
    }
}

MaRC::OblateSpheroid::OblateSpheroid(bool prograde,
                                     double eq_rad,
                                     double pol_rad)
    : BodyData(prograde)
    , eq_rad_(validate_radius(eq_rad))
    , pol_rad_(validate_radius(pol_rad))
    , first_eccentricity_(std::sqrt(1 - std::pow(pol_rad / eq_rad, 2)))
{
    // a >= c for oblate spheroid.
    if (eq_rad < pol_rad)
        throw std::invalid_argument("Equatorial radius is less than "
                                    "polar radius.");
}

double
MaRC::OblateSpheroid::centric_radius(double lat) const
{
    /*
      Given a planetocentric latitude and longitude for a point (x, y, z)
      on the surface of a spheroid:

          x = r * cos(lat) * cos(lon)
          y = r * cos(lat) * sin(lon)
          z = r * sin(lat)

      Assuming we're at longitude zero, we have:

          x = r * cos(lat)
          y = 0
          z = r * sin(lat)

      The following also works when the longitude isn't zero since
      the longitude terms drop out due to the property:

             2           2
          sin (lon) + cos (lon) = 1

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
