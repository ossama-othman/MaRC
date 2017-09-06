// -*- C++ -*-
/**
 * @file SimpleCylindrical.h
 *
 * Copyright (C) 1996-1997, 1999, 2003-2004, 2017-2018  Ossama Othman
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


#ifndef MARC_SIMPLE_CYLINDRICAL_H
#define MARC_SIMPLE_CYLINDRICAL_H

#include <MaRC/MapFactory.h>
#include <MaRC/BodyData.h>

#include <memory>
#include <cassert>


namespace MaRC
{

    struct map_location
    {
        double latitude;
        double longitude;
    };

    /**
     * @class SimpleCylindrical
     *
     * @brief Simple cylindrical concrete map factory.
     *
     * A Simple cylindrical map contains data mapped to a rectangular
     * latitude/longitude array, where the spacing the between a given
     * number of degrees of latitude and longitude is equal.  This
     * projection is more formally known as the Plate Carrée
     * projection, as well as rectangular, equirectangular and
     * equidistant cylindrical.
     */
    class MARC_API SimpleCylindrical : public MapFactory
    {
    public:

        /// @typedef Type of grid passed to @c plot_grid() method.
        using typename MapFactory::grid_type;

        /// @typedef Type of functor passed to @c plot_map() method.
        using typename MapFactory::plot_type;

        /// Constructor.
        /**
         * @param[in] body        Pointer to @c BodyData object
         *                        representing body being mapped.
         * @param[in] lo_lat      Bodycentric lower latitude in
         *                        degrees in simple cylindrical map.
         * @param[in] hi_lat      Bodycentric upper latitude in
         *                        degrees in simple cylindrical map.
         * @param[in] lo_lon      Lower longitude in degrees in simple
         *                        cylindrical map.
         * @param[in] hi_lon      Upper longitude in degrees in simple
         *                        cylindrical map.
         * @param[in] graphic_lat Map bodygraphic latitudes instead of
         *                        bodycentric latitudes.
         */
        SimpleCylindrical(std::shared_ptr<BodyData> body,
                          double lo_lat,
                          double hi_lat,
                          double lo_lon,
                          double hi_lon,
                          bool graphic_lat);

        /// Destructor
        virtual ~SimpleCylindrical() = default;

        /**
         * @name @c MapFactory Methods
         *
         * Methods required by the @c MapFactory abstract base class.
         *
         * @see @c MapFactory
         */
        //@{
        virtual char const * projection_name() const;
        //@}

        /**
         * @class iterator
         *
         * @brief Simple Cylindrical map iterator.
         *
         * An iterator that when dereferenced returns a latitude and
         * longitude to be used for plotting data on a Simple
         * Cylindrical map.
         *
         * The design of the iterator is such that iterator increment,
         * decrement and advancing operations are inexpensive,
         * i.e. O(1), and expensive mapping operations only occur when
         * dereferencing the iterator.
         *
         * @attention This iterator is not thread-safe, and is only
         *            intended to be used by a single thread at any
         *            given time.
         */
        class iterator
        {
        public:

            /**
             * @name Iterator Traits
             *
             * @brief The canonical C++ iterator traits.
             */
            //@{
            using difference_type   = std::ptrdiff_t;
            using value_type        = map_location;
            using pointer           = value_type *;
            using reference         = value_type &;
            using iterator_category = std::random_access_iterator_tag;
            //@}

            /**
             * @brief Constructor.
             *
             * @param[in] samples     Number of samples in map.
             * @param[in] lines       Number of lines   in map.
             * @param[in] body        Pointer to BodyData object
             *                        representing body being mapped.
             * @param[in] lo_lat      Lower latitude  in simple
             *                        cylindrical map.
             * @param[in] hi_lat      Upper latitude  in simple
             *                        cylindrical map.
             * @param[in] lo_lon      Lower longitude in simple
             *                        cylindrical map.
             * @param[in] hi_lon      Upper longitude in simple
             *                        cylindrical map.
             * @param[in] graphic_lat Map bodygraphic latitudes
             *                        instead of bodycentric
             *                        latitudes.
             */
            iterator(std::size_t samples,
                     std::size_t lines,
                     std::shared_ptr<BodyData> body,
                     double lo_lat,
                     double hi_lat,
                     double lo_lon,
                     double hi_lon,
                     bool graphic_lat)
                : samples_(samples)
                , lines_(lines)
                , body_(body)
                , lo_lat_(lo_lat)
                , lo_lon_(lo_lon)
                , hi_lon_(hi_lon)
                , latitudes_per_line_((hi_lat - lo_lat) / lines)
                , longitudes_per_sample_(
                    std::abs(hi_lon - lo_lon) / samples)
                , graphic_lat_(graphic_lat)
            {
                // These values should have already been verified by
                // the map constructor.
                assert(samples > 0);
                assert(lines > 0);
                assert(lo_lat >= -C::pi_2); // -90 degrees
                assert(hi_lat <=  C::pi_2); //  90 degrees
                assert(std::abs(lo_lon) <= C::_2pi);  // +/-360 degrees
                assert(std::abs(hi_lon) <= C::_2pi);  // +/-360 degrees
            }

            // Copy constructor.
            iterator(iterator const & rhs)
                : samples_(rhs.samples_)
                , lines_(rhs.lines_)
                , body_(rhs.body_)
                , lo_lat_(rhs.lo_lat_)
                , lo_lon_(rhs.lo_lon_)
                , hi_lon_(rhs.hi_lon_)
                , latitudes_per_line_(rhs.latitudes_per_line_)
                , longitudes_per_sample_(rhs.longitudes_per_sample_)
                , graphic_lat_(rhs.graphic_lat)
            {
            }

            /// Destructor.
            ~iterator() = default;

            // Copy assignment operator.
            iterator & operator=(iterator const & rhs)
            {
                this->samples_ = rhs.samples_;
                this->lines_   = rhs.lines_;

                this->body_    = rhs.body_;

                this->lo_lat_  = rhs.lo_lat_;
                this->lo_lon_  = rhs.lo_lon_;
                this->hi_lon_  = rhs.hi_lon_;

                this->latitudes_per_line_ = rhs.latitudes_per_line_;
                this->longitudes_per_sample_ = rhs.longitudes_per_sample_;

                this->graphic_lat_ = rhs.graphic_lat_;

                return *this;
            }

            /// Pre-increment operator.
            iterator & operator++()
            {
                ++this->offset_;

                return *this;
            }

            /// Post-increment operator.
            iterator operator++(int)
            {
                auto tmp(*this);

                ++*this;

                return *tmp;
            }

            /// Pre-decrement operator.
            iterator & operator--()
            {
                --this->offset_;

                return *this;
            }

            /// Post-decrement operator.
            iterator operator--(int)
            {
                auto tmp(*this);

                --*this;

                return *tmp;
            }

            /// Addition assignment operator.
            iterator & operator+=(difference_type n)
            {
                this->offset_ += n;

                return *this;
            }

            /// Subtraction assignment operator.
            iterator & operator-=(difference_type n)
            {
                this->offset_ -= n;

                return *this;
            }

            /// Map iterator deference operator.
            /**
             * Conversion from map element (line and sample) to
             * latitude longitude occurs in this operator.
             *
             * @return Reference to the current location (latitude and
             *         longitude) in the map pointed to by this
             *         iteratore.
             */
            reference operator*() const
            {
                if (this->offset_ >= this->samples_ * this->lines_)
                    throw
                        std::out_of_range(
                            "Simple Cylindrical map iterator "
                            "exceeds bounds.");

                auto const line   = this->offset_ / this->samples_;
                auto const sample = this->offset_ % this->samples_;

                auto & lat = this->location_.latitude;
                auto & lon = this->location_.longitude;

                // Compute latitude at center of pixel.
                lat =
                    this->lo_lat_
                    + (line + 0.5) * this->latitudes_per_line_;

                // Convert to CENTRIC latitude if we're mapping
                // GRAPHIC latitudes since MaRC data read operations
                // expect CENTRIC latitudes.
                if (this->graphic_lat_)
                    lat = this->body_->centric_latitude(lat);

                // Compute longitude at center of pixel.
                lon = (sample + 0.5) * this->longitudes_per_sample_;

                // PROGRADE ----> longitudes increase to the left
                // RETROGRADE --> longitudes increase to the right
                if (this->body_->prograde())
                    lon  = this->hi_lon_ - lon;
                else
                    lon += this->lo_lon_;

                return this->location_;
            }

            /**
             * @brief Subscript operator.
             *
             * @param[in] n Offset relative to the current location
             *              to which the iterator points.
             *
             * @return Copy of @c map_location at @a n iterations
             *         away from the current iterator offset.
             *
             * @note A copy of the @c map_location instead of a
             *       reference is returned since a reference cannot be
             *       returned without first advancing the @c iterator
             *       by @a n.  Otherwise we end up with a dangling
             *       reference.
             */
            value_type operator[](difference_type n) const
            {
                /**
                 * @bug This creates a temporary so we have to return
                 *      by value instead of reference.
                 */
                return *(*this + n);
            }

            /**
             * @brief Compare equality of this iterator with another.
             *
             * @param[in] rhs Iterator to be checked for equality with
             *                this one.
             *
             * @return @c true if @c *this and @a rhs are considered
             *         to be equal.
             */
            bool is_equal(iterator const & rhs) const
            {
                static constexpr int ulps = 2;

                // Check offset_ before all others since it'll be
                // quick to do so, and since it is likely to be
                // different.  Perform the more expensive comparisons
                // the cheap ones to take advantage of short
                // circuiting logic.
                return
                    this->offset_ == rhs.offset_
                    && this->samples_ == rhs.samples_
                    && this->lines_ == rhs.lines_
                    && graphic_lat_ == rhs.graphic_lat_
                    && MaRC::almost_equal(this->latitudes_per_line_,
                                          rhs.latitudes_per_line_,
                                          ulps)
                    && MaRC::almost_equal(this->longitudes_per_sample_,
                                          rhs.longitudes_per_sample_,
                                          ulps)
                    && this->body_ == rhs.body_
                    && MaRC::almost_equal(this->lo_lat_,
                                          rhs.lo_lat_,
                                          ulps)
                    && MaRC::almost_equal(this->lo_lon_,
                                          rhs.lo_lon_,
                                          ulps)
                    && MaRC::almost_equal(this->hi_lon_,
                                          rhs.hi_lon_,
                                          ulps);
            }

            /**
             * @brief Determine if this iterator is less than
             *        another.
             *
             * @param[in] rhs Iterator to be compared against.
             *
             * @return @c true if @c *this is less than @a rhs.
             */
            bool is_less(iterator const & rhs) const
            {
                static constexpr int ulps = 2;

                // Check offset_ before all others since it'll be
                // quick to do so, and since it is likely to be
                // different.  Perform the more expensive comparisons
                // the cheap ones to take advantage of short
                // circuiting logic.

                // *this < rhs if this->offset_ is less, and
                // everything else is the same.
                return
                    this->offset_ < rhs.offset_
                    && this->samples_ == rhs.samples_
                    && this->lines_ == rhs.lines_
                    && graphic_lat_ == rhs.graphic_lat_
                    && MaRC::almost_equal(this->latitudes_per_line_,
                                          rhs.latitudes_per_line_,
                                          ulps)
                    && MaRC::almost_equal(this->longitudes_per_sample_,
                                          rhs.longitudes_per_sample_,
                                          ulps)
                    && this->body_ == rhs.body_
                    && MaRC::almost_equal(this->lo_lat_,
                                          rhs.lo_lat_,
                                          ulps)
                    && MaRC::almost_equal(this->lo_lon_,
                                          rhs.lo_lon_,
                                          ulps)
                    && MaRC::almost_equal(this->hi_lon_,
                                          rhs.hi_lon_,
                                          ulps);
            }

        private:

            /// The number of samples in the map.
            std::size_t samples_;

            /// The number of lines in the map.
            std::size_t lines_;

            /// Number of latitudes per line in the map.
            double latitudes_per_line_;

            /// Number of longitudes per sample in the map.
            double longitudes_per_sample_;

            /**
             * Zero-based linear offset in the map array.  For
             * example, an offset of 7 in a map with 5 samples and 5
             * lines corresponds to line 1 and sample 2, where line 0
             * and sample 0 are the first row and column,
             * respectively.
             */
            std::size_t offset_;

            /**
             * @brief Current map latitude and longitude.
             *
             * Object containing the current latitude and longitude
             * pointed to by this iterator.  The latitude and
             * longitude will only be updated when the @c iterator is
             * dereferenced.
             */
            mutable map_location location_;

            /// Pointer to @c BodyData object representing body being
            /// mapped.
            std::shared_ptr<BodyData> body_;

            /// Lower latitude in simple cylindrical map.
            double lo_lat_;

            /// Lower longitude in simple cylindrical map.
            double lo_lon_;

            /// Upper longitude in simple cylindrical map.
            double hi_lon_;

            /// Flag that determines if bodygraphic latitudes are
            /// mapped instead of bodycentric latitudes.
            bool graphic_lat_;

        };

        /**
         * @brief "Begin" iterator factory method.
         *
         * Create a Simple Cylindrical map iterator that points to the
         * beginning of the map.
         *
         * @param[in] samples Number of samples in map.
         * @param[in] lines   Number of lines   in map.
         *
         * @return @c iterator that points to the beginning of the
         *         map.
         */
        iterator begin(std::size_t samples,
                       std::size_t lines) const
        {
            iterator i(samples,
                       lines,
                       this->body_,
                       this->lo_lat_,
                       this->lo_lon_,
                       this->hi_lat_,
                       this->hi_lon_,
                       this->graphic_lat_);

            return i;
        }

        /**
         * @brief "End" iterator factory method.
         *
         * Create a Simple Cylindrical map iterator that points to the
         * end of the map, i.e. one past the last element in the map.
         *
         * @param[in] samples Number of samples in map.
         * @param[in] lines   Number of lines   in map.
         *
         * @return @c iterator that points to the end of the
         *         map.
         */
        iterator end(std::size_t samples,
                     std::size_t lines) const
        {
            iterator i(this->begin(samples, lines));

            // Move to the end.
            std::advance(i, samples * lines);

            return i;
        }

    private:

        /**
         * Create the Simple Cylindrical map projection.
         *
         * @see @c MaRC::MapFactory::plot_map().
         */
        virtual void plot_map(std::size_t samples,
                              std::size_t lines,
                              plot_type plot) const;

        /**
         * Create the Simple Cylindrical map latitude/longitude grid.
         *
         * @see @c MaRC::MapFactory::plot_grid().
         */
        virtual void plot_grid(std::size_t samples,
                               std::size_t lines,
                               float lat_interval,
                               float lon_interval,
                               grid_type & grid) const;

        /// Orient longitude according to rotation direction
        /// (prograde/retrograde).
        /**
         * @param[in] i       Sample in map being mapped.
         * @param[in] samples Number of samples in image.
         */
        inline double get_longitude(std::size_t i,
                                    std::size_t samples) const
        {
            // Compute longitude at center of pixel.

            double lon =
                (i + 0.5) / samples * (this->hi_lon_ - this->lo_lon_);

            // PROGRADE ----> longitudes increase to the left
            // RETROGRADE --> longitudes increase to the right

            if (this->body_->prograde ())
                lon = this->hi_lon_ - lon;
            else
                lon += this->lo_lon_;

            return lon;
        }

    private:

        /// @c BodyData object representing the body being mapped.
        std::shared_ptr<BodyData> const body_;

        /// Lower latitude in simple cylindrical map.
        double lo_lat_;

        /// Upper latitude in simple cylindrical map.
        double hi_lat_;

        /// Lower longitude in simple cylindrical map.
        double lo_lon_;

        /// Upper longitude in simple cylindrical map.
        double hi_lon_;

        /// Flag that determines if bodygraphic latitudes are mapped
        /// instead of bodycentric latitudes.
        bool const graphic_lat_;

    };


    /// Equality operator.
    bool operator==(SimpleCylindrical::iterator const & lhs,
                    SimpleCylindrical::iterator const & rhs)
    {
        return lhs.is_equal(rhs);
    }

    /// Less-than operator.
    bool operator<(SimpleCylindrical::iterator const & lhs,
                   SimpleCylindrical::iterator const & rhs)
    {
        return lhs.is_less(rhs);
    }

    /// Addition operator.
    auto operator+(SimpleCylindrical::iterator lhs,
                   SimpleCylindrical::iterator const & rhs)
    {
        lhs += rhs;

        return lhs;
    }

    /// Subtraction operator.
    auto operator-(SimpleCylindrical::iterator lhs,
                   SimpleCylindrical::iterator const & rhs)
    {
        lhs -= rhs;

        return tmp;
    }
}


#endif
