/**
 * @file Vector_Test.cpp
 *
 * Copyright (C) 2017 Ossama Othman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * by the Free Software Foundation; either version 2 of the License, or
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
 */

#include <MaRC/ViewingGeometry.h>
#include <MaRC/OblateSpheroid.h>
#include <MaRC/Constants.h>


namespace
{
    // Jupiter
    constexpr bool   prograde   = true;
    constexpr double eq_rad     = 71492;
    constexpr double pol_rad    = 66854;

    std::shared_ptr<MaRC::OblateSpheroid> body =
        std::make_shared<MaRC::OblateSpheroid>(prograde, eq_rad, pol_rad);

    // Viewing geometry parameters.
    constexpr double sample_center = 2807.61;  // pixels
    constexpr double line_center   = 1200.67;
    constexpr double sub_obs_lat   = -15.63;   // degrees
    constexpr double sub_obs_lon   = -144.37;
    constexpr double pos_angle     = 27.175;
    constexpr double sub_sol_lat   = 0.22;
    constexpr double sub_sol_lon   = 75.33;
    constexpr double range         = 1211230;  // kilometers
    constexpr double focal_length  = 1501.039; // mm
    constexpr double pixel_scale   = 32.8084;  // pixels / mm

    // "Image" size
    constexpr std::size_t image_samples = 400; // pixels
    constexpr std::size_t image_lines   = 200;
}

bool test_initialization(MaRC::ViewingGeometry & vg)
{
    vg.body_center(sample_center, line_center);
    vg.sub_observ(sub_obs_lat, sub_obs_lon);
    vg.position_angle(pos_angle);
    vg.sub_solar(sub_sol_lat, sub_sol_lon);
    vg.range(range);
    vg.focal_length(focal_length);
    vg.scale(pixel_scale);

    vg.finalize_setup(image_samples, image_lines);

    return true;
}

bool test_visibility(MaRC::ViewingGeometry & vg)
{
    // Point on the far side of the planet.  Not visible to the
    // observer.  This is suitable for bodies modelled as oblate
    // spheroids.
    constexpr double far_lat = -sub_obs_lat * C::degree;
    constexpr double far_lon = (sub_obs_lon + 180) * C::degree;

    double sample, line;

    /**
     * @todo Test visibility of points on the near side of the body.
     *
     * @todo Test lack of visibility in points on the body that reside
     *       beyond the terminator, i.e. in the dark side of the
     *       body.
     *
     * @note The sub-observation point does not necessarily correspond
     *       to the latitude and longitude of the point on the surface
     *       of the body through which the optical axis intersects.
     *       The camera, for example, may be pointed away from the
     *       sub-observation point.
     */

    return !vg.latlon2pix(far_lat, far_lon, sample, line);
}

bool test_conversion(MaRC::ViewingGeometry & vg)
{
    double sample, line;
    double lat, lon;

    /**
     * @attention @c latlon2pix() (and is_visible()) return @c true
     *            when the point on the surface of the body is on the
     *            near side of the body rather than the far side.
     */
    bool const visible =
        vg.latlon2pix(sub_obs_lat * C::degree,
                      sub_obs_lon * C::degree,
                      sample,
                      line)
        && vg.pix2latlon(sample, line, lat, lon);

    /*
      The ulp values below vary greatly due to the magnitudes of
      values being compared.  Some of the ulp values may seem huge but
      the actual floating point values being compared are well within
      the desired margin of error, e.g.: -15.630000000000001 vs
      -15.629999999999912 for the sub-observation latitude conversion
      check.
    */
    return
        visible
        && MaRC::almost_equal(sample_center, sample, 4)
        && MaRC::almost_equal(line_center, line, 12)
        && MaRC::almost_equal(sub_obs_lat, lat / C::degree, 13)
        && MaRC::almost_equal(sub_obs_lon < 0
                              ? sub_obs_lon + 360
                              : sub_obs_lon,
                              lon / C::degree, 4);
}


int main()
{
    MaRC::ViewingGeometry vg(body);

    return
        test_initialization(vg)
        && test_visibility(vg)
        && test_conversion(vg)
        ? 0 : -1;
}
