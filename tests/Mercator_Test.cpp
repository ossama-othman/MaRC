/**
 * @file Mercator_Test.cpp
 *
 * Copyright (C) 2018, 2020 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <marc/Mercator.h>
#include <marc/OblateSpheroid.h>
#include <marc/LatitudeImage.h>
#include <marc/Constants.h>
#include <marc/DefaultConfiguration.h>
#include <marc/scale_and_offset.h>

#include <catch2/catch.hpp>

#include <memory>
#include <cstring>
#include <ctime>
#include <random>
#include <algorithm>


namespace
{
    constexpr bool   prograde   = true;
    constexpr double eq_rad     = 1234567;
    constexpr double pol_rad    = eq_rad / 2;

    std::shared_ptr<MaRC::OblateSpheroid> body =
        std::make_shared<MaRC::OblateSpheroid>(prograde, eq_rad, pol_rad);

    auto projection =
        std::make_unique<MaRC::Mercator>(body);

    /*
      Choose an odd value for the map height so that the center of the
      map falls on the center of a pixel.  This allows us to
      accurately locate the body's equator in the Mercator projection
      since MaRC maps data at the center of map pixel.  For example,
      for the pixel (line, sample) = (0, 0) MaRC maps data at pixel
      coordinate (0.5, 0.5), i.e. the center of the pixel.
     */
    constexpr std::size_t samples = 50;
    constexpr std::size_t lines   = 61;

    template <typename T>
    constexpr bool is_odd(T n)
    {
        return n % 2;
    }

    static_assert(is_odd(lines),
                  "Map lines should be odd for this test.");

}

/**
 * @test Test the MaRC::Mercator::projection_name() method.
 */
TEST_CASE("Mercator projection name", "[mercator]")
{
    static char const name[] = "Mercator";

    REQUIRE(std::strcmp(projection->projection_name(), name) == 0);
}

/**
 * @test Test the MaRC::Mercator::make_map() method, i.e. Mercator map
 *       projection image creation.
 */
TEST_CASE("Make Mercator projection map", "[mercator]")
{
    /**
     * @todo Test conformal properties of Mercator projection.
     */

    using namespace MaRC::default_configuration;

    using data_type = double;

    // Scale and offset used to make physical data (latitudes) fit in
    // map data type.
    double map_scale;
    double map_offset;

    REQUIRE(MaRC::scale_and_offset<data_type>(latitude_low,
                                              latitude_high,
                                              map_scale,
                                              map_offset));

    constexpr bool graphic_latitudes = false;

    auto const image =
        std::make_unique<MaRC::LatitudeImage>(body,
                                              graphic_latitudes,
                                              map_scale,
                                              map_offset);

    /**
     * @todo Verify that the longitudes are correct by taking into
     *       account the body rotation.
     */

    constexpr auto minimum = std::numeric_limits<data_type>::lowest();
    constexpr auto maximum = std::numeric_limits<data_type>::max();

    MaRC::plot_info info(*image, minimum, maximum);

    auto const map =
        projection->template make_map<data_type>(info, samples, lines);

    REQUIRE_FALSE(map.empty());

    /*
      Pick a random sample along the center line (equator) in the
      projection.  There is no need for a non-deterministically
      generated seed value for the purposes of this test.
    */
    std::mt19937 generator(std::time(nullptr));
    std::uniform_int_distribution<std::size_t> distribution(0,
                                                            samples - 1);

    auto const     sample         = distribution(generator);
    constexpr auto equator_line   = lines / 2;
    auto const     equator_offset = equator_line * samples + sample;
    auto const     equator_data   =
        map[equator_offset] * image->scale() + image->offset();

    /**
     * @todo @c equator_data is close to zero as expected (on the
     *       order of 1e-15.  Can we somehow get even closer to zero
     *       so that we can reduce this @c ulps value?
     */
    constexpr int ulps = 30;

    REQUIRE(MaRC::almost_zero(equator_data, ulps));
}

/**
 * @test Test the MaRC::Mercator::make_grid() method, i.e. Mercator
 *       grid image creation.
 */
TEST_CASE("Make Mercator projection grid", "[mercator]")
{
    constexpr auto lat_interval = 10;
    constexpr auto lon_interval = 10;

    auto const grid =
        projection->make_grid(samples, lines, lat_interval, lon_interval);

    auto const minmax =
        std::minmax_element(grid.begin(), grid.end());

    static constexpr auto black =
        std::numeric_limits<decltype(grid)::value_type>::lowest();

    static constexpr auto white =
        std::numeric_limits<decltype(grid)::value_type>::max();

    REQUIRE(minmax.first != minmax.second);
    REQUIRE(*minmax.first  == black);
    REQUIRE(*minmax.second == white);
}

/**
 * @test Test the MaRC::Mercator::distortion() method, i.e. scale
 *       distortion in the Mercator map.
 */
TEST_CASE("Mercator projection scale distortion", "[mercator]")
{
    // Latitude at the center of the map.
    constexpr double equator     = 0;

    // Arbitrary planetographic latitude that is not the equator.
    constexpr double not_equator = 30 * C::degree;

    // Scale distortion at along the equator in the Transverse
    // Mercator projection should be 1.
    constexpr double equator_distortion = 1;

    constexpr int ulps = 2;

    auto const distortion = projection->distortion(equator);

    REQUIRE(MaRC::almost_equal(equator_distortion, distortion, ulps));

    // Distortion away from the equator should always be greater than
    // one.
    REQUIRE(projection->distortion(not_equator) > equator_distortion);
}
