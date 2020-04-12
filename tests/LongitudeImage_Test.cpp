/**
 * @file LongitudeImage_Test.cpp
 *
 * Copyright (C) 2017, 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/LongitudeImage.h>
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
     * @brief Validate longitude values obtained from
     *        MaRC::LongitudeImage.
     *
     * @param[in] longitude_image MaRC::LongitudeImage object.
     * @param[in] expected_lon    Longitude in degrees expected to be
     *                            returned from
     *                            MaRC::LongitudeImage::read_data().
     * @param[in] test_lon        Longitude in radians that was passed
     *                            as the longitude argument to
     *                            MaRC::LongitudeImage::read_data().
     *
     * @retval true  Test succeeded.
     * @retval false Test failed.
     */
    bool test_read_data(
    std::unique_ptr<MaRC::VirtualImage> const & longitude_image,
    double expected_lon,  // degrees
    double test_lon)      // radians
    {
        constexpr double latitude = 72 * C::degree;  // arbitrary
        constexpr int    ulps     = 2;

        double data;  // Data will be in degrees.

        // Scale and offset used to convert map data to physical data
        // (longitudes)
        double const data_scale  = longitude_image->scale();
        double const data_offset = longitude_image->offset();

        bool const visible =
            longitude_image->read_data(latitude, test_lon, data);

        // Do not blindly use operator*=() here!
        if (visible)
            data = data * data_scale + data_offset;

        return
            visible
            && (MaRC::almost_equal(expected_lon, data, ulps)

                // Case where expected longitude is 360 and longitude
                // returned from LongitudeImage is 0.  Both are
                // equivalent.
                || (MaRC::almost_equal(expected_lon, 360.0, ulps)
                    && MaRC::almost_equal(expected_lon,
                                          data + 360,
                                          ulps))

                // Case where longitude is essentially zero may fail
                // the above equality test due to limitations in
                // MaRC::almost_equal().  Check if both values are
                // almost zero instead.
                || (MaRC::almost_zero(expected_lon, ulps)
                    /**
                     * @note 2 ulps isn't enough on some 32 bit
                     *       platforms.  For example, this test fails
                     *       on a 32 bit virtual machine since the
                     *       data value is about 3.747e-15 but the
                     *       epsilon() is about 2.2204e-16.  Use an
                     *       ulps value that allows this test to pass
                     *       since 3.747e-15 is essentially zero for
                     *       this use case.
                     */
                    && MaRC::almost_zero(data, 17)));
    }
}

/**
 * @test Test the MaRC::LongitudeImage class.
 */
TEMPLATE_TEST_CASE("MaRC::LongitudeImage",
                   "[longitude image]",
                   int16_t,
                   uint32_t,
                   float,
                   double)
{
    using namespace MaRC::default_configuration;

    // Scale and offset used to make physical data (longitudes) fit in
    // map data type.
    double map_scale;
    double map_offset;

    REQUIRE(MaRC::scale_and_offset<TestType>(longitude_low,
                                             longitude_high,
                                             map_scale,
                                             map_offset));

    std::unique_ptr<MaRC::VirtualImage> const longitude_image =
        std::make_unique<MaRC::LongitudeImage>(map_scale, map_offset);

    // Lower and upper longitude bounds in degrees.
    // Longitude test values in radians.
    constexpr double lo_lon  = longitude_low  * C::degree;
    constexpr double hi_lon  = longitude_high * C::degree;
    constexpr double mid_lon = (hi_lon - lo_lon) / 2;

    // Test wrap-around
    constexpr double shift   = C::pi_4;  // radians
    constexpr double oob_lon = lo_lon - shift;  // out-of-bounds
    constexpr double ib_lon  = hi_lon - shift;  // in-bounds

    // Expected unit string.
    constexpr char const unit[] = "deg"; // Per FITS recommendation.

    REQUIRE(longitude_image);

    REQUIRE(test_read_data(longitude_image, longitude_low, lo_lon));
    REQUIRE(test_read_data(longitude_image, longitude_high, hi_lon));
    REQUIRE(test_read_data(longitude_image, mid_lon / C::degree, mid_lon));
    REQUIRE(test_read_data(longitude_image, ib_lon  / C::degree, oob_lon));

    REQUIRE(std::strcmp(longitude_image->unit(), unit) == 0);
}
