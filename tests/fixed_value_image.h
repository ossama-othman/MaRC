// -*- C++ -*-
/**
 * @file fixed_value_image.h
 *
 * Copyright (C) 2021  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_FIXED_VALUE_IMAGE_H
#define MARC_FIXED_VALUE_IMAGE_H

#include <marc/SourceImage.h>
#include <marc/Validate.h>
#include <marc/Constants.h>


namespace MaRC
{
    // Latitudes and longitudes passed to constructors in degrees.

    /**
     * @class point
     *
     * @brief A latitude / longitude pair.
     */
    class point
    {
    public:

        /**
         * @brief Constructor.
         *
         * @param[in] lat Latitude in degrees at which data should be
         *                composited.
         * @param[in] lon Longitude in degrees at which data should be
         *                composited.
         */
        point(double lat, double lon)
            : lat_(validate_latitude(lat))
            , lon_(validate_longitude(lon))
        {
        }

        point(point const &) = default;
        point & operator=(point const &) = default;

        /// Latitude in radians.
        double lat() const { return this->lat_; }

        /// Longitude in radians.
        double lon() const { return this->lon_; }

    private:

        /// Latitude in radians.
        double const lat_;

        /// Longitude in radians.
        double const lon_;

    };

    /**
     * @class fixed_value_image
     *
     * @brief Test image that only returns a single value.
     *
     * This test image returns a single value within a given "square"
     * shaped latitude and longitude range.
     */
    class fixed_value_image final : public MaRC::SourceImage
    {
    public:

        /**
         * @brief Constructor
         *
         * @param[in] lower_corner Lower corner (lat/lon) of test
         *                         image.
         * @param[in] edge_length  Length of each side of the image.
         * @param[in] value        Value found in the "visible"
         *                         lat/lon range of the image.
         */
        fixed_value_image(point const & lower_corner,
                          double edge_length,
                          double value)
            : SourceImage()
            , lower_lat_(lower_corner.lat())
            , lower_lon_(lower_corner.lon())
            , upper_lat_(get_upper_lat(lower_corner.lat(), edge_length))
            , upper_lon_(get_upper_lon(lower_corner.lon(), edge_length))
            , value_(value)
        {
        }

        ~fixed_value_image() override = default;

        fixed_value_image(fixed_value_image const &) = delete;
        fixed_value_image & operator=(fixed_value_image const &) = delete;
        fixed_value_image(fixed_value_image &&) = delete;
        fixed_value_image & operator=(fixed_value_image &&) = delete;

        /**
         * @brief Return the value passed to the constructor.
         *
         * @see MaRC::SourceImage::read_data().
         */
        bool read_data(double lat,
                       double lon,
                       double & data) const override;

        /**
         * @brief Return the value passed to the constructor.
         *
         * Return the value and passed to the constructor as well as
         * a data weight based on proximity of the given @a lat and
         * @a lon to the image edges.
         *
         * @see MaRC::SourceImage::read_data().
         */
        bool read_data(double lat,
                       double lon,
                       double & data,
                       double & weight,
                       bool scan) const override;

    private:

        /// Get upper latitude boundary.
        double get_upper_lat(double lower_lat, double edge_length) const
        {
            // Cannot have latitude great than 90 degrees.  Clamp as
            // needed.
            constexpr double max_lat = C::pi_2;

            edge_length *= C::degree; // radians

            double const upper_lat = lower_lat + edge_length;

            return upper_lat > max_lat ? max_lat : upper_lat;
        }

        /// Get upper longitude boundary.
        double get_upper_lon(double lower_lon, double edge_length) const
        {
            edge_length *= C::degree; // radians

            /*
              Allow equivalent upper longitude beyond 360 degrees.
              For example given a lower longitude of 20 degrees, an
              an upper longitude of 10 degrees, and a longitude X=5
              degrees, the longitude X is actually in the longitude
              range [20, 10]:

                lower longitude = 20
                upper longitude = 10 + 360 = 370

                if (X < lower longitude)
                   X += 360
                else if (X > upper longitude)
                   X -= 360

                X = 5 or 365  (both equivalent since 365 % 360 = 5)

                -X-- 10   20 -----------------
                          20 -------------------- 370

                20 <= 365 <= 370    in range!
            */
            return lower_lon + edge_length;
        }

    private:

        /// Lower image latitude (radians).
        double const lower_lat_;

        /// Lower image longitude (radians).
        double const lower_lon_;

        /// Upper image latitude (radians).
        double const upper_lat_;

        /// Upper image longitude (radians).
        double const upper_lon_;

        /// Data value within image bounds.
        double const value_;

    };

} // End MaRC namespace


#endif  /* MARC_FIXED_VALUE_IMAGE_H */
