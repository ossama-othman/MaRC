/**
 * @file compositing_strategy_test.cpp
 *
 * Copyright (C) 2021 Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "fixed_value_image.h"

#include <marc/first_read.h>
#include <marc/unweighted_average.h>
#include <marc/weighted_average.h>
#include <marc/Mathematics.h>


class test_data
{
public:

    /**
     * @brief Constructor.
     *
     * @param[in] lat   Latitude in degrees at which data should be
     *                  composited.
     * @param[in] lon   Longitude in degrees at which data should be
     *                  composited.
     * @param[in] count Expected number of images that contributed to
     *                  the composite.
     * @param[in] data  Expected value of the composited data.
     */
    test_data(double const lat,
              double const lon,
              int count,
              double data)
        : point_(lat, lon)
        , count_(count)
        , data_(data)
    {
    }

    /// Latitude in radians at which data should be composited.
    auto lat() const { return this->point_.lat(); }

    /// Longitude in radians at which data should be composited.
    auto lon() const { return this->point_.lon(); }

    /// Expected number of images that contributed to the composite.
    auto count() const { return this->count_; }

    bool check_data(double data) const
    {
        constexpr int ulps = 2;

        return MaRC::almost_equal(data, this->data_, ulps)
            || (MaRC::almost_zero(data, ulps)
                && MaRC::almost_zero(this->data_, ulps));
    }

private:

    /// Lat/lon point in radians at which data should be composited.
    MaRC::point const point_;

    /// Expected number of images that contributed to the composite.
    int const count_;

    /// Expected value of the composited data.
    double const data_;

};

/**
 * @test Test the MaRC::first_read compositing strategy.
 */
bool
test_first_read(MaRC::compositing_strategy::list_type const & images)
{
    test_data const tds[] = {
        {  5,  5, 0,   0 },  // Not inside an image.
        { 12, 20, 1, 100 },  // I
        { 20, 20, 1, 100 },  // I and III
        { 20, 27, 1, 100 }   // I, II, and III
    };

    MaRC::first_read cs;

    for (auto const & td : tds) {
        double data = 0;

        if (cs.composite(images, td.lat(), td.lon(), data) != td.count()
            || !td.check_data(data))
            return false;
    }

    return true;
}

/**
 * @test Test the MaRC::unweighted_average compositing strategy.
 */
bool
test_unweighted_average(MaRC::compositing_strategy::list_type const & images)
{
    test_data const tds[] = {
        {  5,  5, 0,   0 },  // Not inside an image.
        { 12, 20, 1, 100 },  // I
        { 10, 35, 1, 200 },  // II
        { 33, 33, 1, 300 },  // III
        { 12, 27, 2, 150 },  // I and II
        { 20, 20, 2, 200 },  // I and III
        { 20, 33, 2, 250 },  // II and III
        { 20, 27, 3, 200 }   // I, II, and III
    };

    MaRC::unweighted_average cs;

    for (auto const & td : tds) {
        double data = 0;

        if (cs.composite(images, td.lat(), td.lon(), data) != td.count()
            || !td.check_data(data))
            return false;
    }

    return true;
}

/**
 * @test Test the MaRC::weighted_average compositing strategy.
 */
bool
test_weighted_average(MaRC::compositing_strategy::list_type const & images)
{
    test_data const tds[] = {
        {  5,  5, 0,   0 },  // Not inside an image.
        { 12, 20, 1, 100 },  // I
        { 10, 35, 1, 200 },  // II
        { 33, 33, 1, 300 },  // III
        { 12, 27, 2, 150 },  // I and II
        { 21, 21, 2, 180 },  // I and III
        { 22, 33, 2, 240 },  // II and III
        { 20, 27, 3, 220 }   // I, II, and III
    };

    MaRC::weighted_average cs;

    for (auto const & td : tds) {
        double data = 0;

        if (cs.composite(images, td.lat(), td.lon(), data) != td.count()
            || !td.check_data(data))
            return false;
    }

    return true;
}

/// The canonical main entry point.
int main()
{
    /*
                 35 +-----------------+
                    |                 |
                    |           III   |
        30 +--------|--------+        |
           |        |        |        |
           |  I     |   +-------------|---+ 25
           |        |   |    |        |   |
           |     15 +-----------------+   |
           |        15  |    |        35  |
           |            |    |            |
        10 +------------|----+     II     |
           10           |    30           |
                        +-----------------+ 5
                        25                45
    */
    MaRC::point corners[] = {
        { 10, 10 },  // I
        {  5, 25 },  // II
        { 15, 15 }   // III
    };

    constexpr double edge_length = 20;  // degrees

    std::vector<test_data> td;
    MaRC::compositing_strategy::list_type images;

    for (std::size_t i = 0; i < std::size(corners); ++i) {
        double const value = (i + 1) * 100;

        images.emplace_back(
            std::make_unique<MaRC::fixed_value_image>(corners[i],
                                                      edge_length,
                                                      value));
    }

    return test_first_read(images)
        && test_unweighted_average(images)
        && test_weighted_average(images)
        ? 0 : -1;
}
