/**
 * @file LatitudeImage_Test.cpp
 *
 * Copyright (C) 2017-2018, 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/LatitudeImage.h>
#include <marc/OblateSpheroid.h>
#include <marc/DefaultConfiguration.h>
#include <marc/Mathematics.h>
#include <marc/Constants.h>
#include <marc/scale_and_offset.h>

#include <catch2/catch.hpp>

#include <memory>
#include <cstdint>
#include <cstring>


namespace
{
    /**
     * @brief Validate latitude values obtained from
     *        MaRC::LatitudeImage.
     *
     * @param[in] latitude_image MaRC::LatitudeImage object.
     * @param[in] expected_lat   Latitude in degrees expected to be
     *                           returned from
     *                           MaRC::LatitudeImage::read_data().
     * @param[in] test_lat       Latitude in radians that was passed
     *                           as the latitude argument to
     *                           MaRC::LatitudeImage::read_data().
     *
     * @retval true  Test succeeded.
     * @retval false Test failed.
     */
    bool test_read_data(
        std::unique_ptr<MaRC::VirtualImage> const & latitude_image,
        double expected_lat,  // degrees
        double test_lat)      // radians
    {
        constexpr double longitude = -42 * C::degree;  // arbitrary
        constexpr int    ulps      = 2;

        double data;  // Data will be in degrees.

        // Scale and offset used to convert map data to physical data
        // (latitudes)
        double const data_scale  = latitude_image->scale();
        double const data_offset = latitude_image->offset();

        return
            latitude_image->read_data(test_lat, longitude, data)
            && MaRC::almost_equal(expected_lat,
                                  data * data_scale + data_offset,
                                  ulps);
    }
}

/**
 * @test Test the MaRC::LatitudeImage class.
 */
TEMPLATE_TEST_CASE("MaRC::LatitudeImage",
                   "[latitude image]",
                   int16_t,
                   uint32_t,
                   float,
                   double)
{
    using namespace MaRC::default_configuration;

    // Scale and offset used to make physical data (latitudes) fit in
    // map data type.
    double map_scale;
    double map_offset;

    REQUIRE(MaRC::scale_and_offset<TestType>(latitude_low,
                                             latitude_high,
                                             map_scale,
                                             map_offset));

    constexpr bool   prograde   = false;
    constexpr double eq_rad     = 1234567;
    constexpr double pol_rad    = eq_rad / 2;

    std::shared_ptr<MaRC::BodyData> body =
        std::make_shared<MaRC::OblateSpheroid>(prograde, eq_rad, pol_rad);

    /**
     * @todo Test case when planetographic latitudes is requested.
     */
    constexpr bool graphic_latitudes = false;

    std::unique_ptr<MaRC::VirtualImage> const latitude_image =
        std::make_unique<MaRC::LatitudeImage>(body,
                                              graphic_latitudes,
                                              map_scale,
                                              map_offset);

    // Lower and upper latitude bounds in degrees.
    // Latitude test values in radians.
    constexpr double lo_lat  = latitude_low  * C::degree;
    constexpr double hi_lat  = latitude_high * C::degree;
    constexpr double mid_lat = (hi_lat - lo_lat) / 2;

    constexpr double oob_lo_lat = -C::pi_2 - 1;  // Out-of-bounds
    constexpr double oob_hi_lat =  C::pi_2 + 1;

    // Expected unit string.
    constexpr char const unit[] = "deg"; // Per FITS recommendation.

    REQUIRE(latitude_image);

    REQUIRE(test_read_data(latitude_image, latitude_low, lo_lat));
    REQUIRE(test_read_data(latitude_image, latitude_high, hi_lat));
    REQUIRE(test_read_data(latitude_image, mid_lat / C::degree, mid_lat));

    REQUIRE(!test_read_data(latitude_image,
                            oob_lo_lat / C::degree,
                            oob_lo_lat));
    REQUIRE(!test_read_data(latitude_image,
                            oob_hi_lat / C::degree,
                            oob_hi_lat));

    REQUIRE(std::strcmp(latitude_image->unit(), unit) == 0);
}
