// -*- C++ -*-
/**
 * @file map_parameters.h
 *
 * Copyright (C) 2018-2019  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_MAP_PARAMETERS_H
#define MARC_MAP_PARAMETERS_H

#include <marc/plot_info.h>

#include <optional>
#include <list>
#include <string>
#include <memory>
#include <set>


namespace MaRC
{
    /**
     * @class map_parameters
     *
     * @brief Map configuration parameters.
     *
     * The map parameters in this class may be supplied by the user or
     * populated automatically from relevant values in the source
     * images being mapped.
     */
    class map_parameters
    {
    public:
        /// %FITS file comment list type.
        using comment_list_type = std::list<std::string>;

        /**
         * @brief Constructor
         *
         * @param[in] plane User-supplied parameters @plane == 0,
         *                  otherwise @a plane > 0 for plane-specific
         *                  (automatically populated parameters).
         *
         * @todo Revise the design so that this constructor and the
         *       @c plane_ are not needed.
         */
        explicit map_parameters(int plane);

        /**
         * @brief Default Coonstructor
         *
         * Convenience constructor used when creating user supplied
         * map parameters.
         *
         */
        map_parameters();

        /// Destructor.
        ~map_parameters() = default;

        // Disallow copying.
        map_parameters(map_parameters const &) = delete;
        void operator=(map_parameters const &) = delete;

        /// Set map author.
        void author(std::string a);

        /// Get map author.
        std::string const & author() const { return this->author_; }

        /**
         * @brief Set map bits per pixel.
         *
         * @li @c   8  8 bit unsigned integer data.
         * @li @c  16 16 bit signed   integer data.
         * @li @c  32 32 bit signed   integer data.
         * @li @c  64 64 bit signed   integer data.
         * @li @c -32 32 bit floating point   data.
         * @li @c -64 64 bit floating point   data.
         *
         * @param[in] n Bits per pixel value as defined by the %FITS
         *              standard.
         */
        void bitpix(int n);

        /**
         * @brief Get map %FITS bits per pixel code.
         *
         * Obtain the bits per pixel "BITPIX" in the map %FITS file,
         * as defined by the %FITS standard.  This value may either be
         * supplied by the user or determined at run-time based on
         * source image data being mapped.
         *
         * @retval   8  8 bit unsigned integer data.
         * @retval  16 16 bit signed   integer data.
         * @retval  32 32 bit signed   integer data.
         * @retval  64 64 bit signed   integer data.
         * @retval -32 32 bit floating point   data.
         * @retval -64 64 bit floating point   data.
         *
         * @throw std::runtime_error Unable to determine bitpix
         *                           value.
         */
        int bitpix() const;

        /**
         * @brief Set the value for the map %FITS @c BLANK keyword.
         *
         * The %FITS @c BLANK keyword only applies to %FITS images
         * containing integer types.  The corresponding "blank" value
         * for floating point %FITS images is the IEEE "not-a-number
         * constant.
         *
         * The @c BLANK keyword merely documents which physical (not
         * %FITS) values in the image array are undefined.
         *
         * @param[in] blank %FITS @c BLANK keyword value.
         */
        void blank(blank_type blank);

        /// Get the value for the map %FITS @c BLANK keyword.
        blank_type blank() const { return this->blank_; }

        /**
         * @brief Set the value for the map %FITS @c BSCALE keyword.
         *
         * The default value of the %FITS @c BSCALE keyword is one.
         *
         * @param[in] scale @c BSCALE keyword value.
         *
         * @note Setting this value will cause the data written to the
         *       %FITS file to be transformed according the equation:
         * @code{.cpp}
         *     (FITS value) = ((physical value) - BZERO) / BSCALE
         * @endcode
         */
        void bscale(std::optional<double> scale);

        /// Get the value for the map %FITS @c BSCALE keyword.
        std::optional<double> bscale() const { return this->bscale_; }

        /**
         * @brief Set the unit of physical data.
         *
         * Get the units of the physical data, i.e. image data that
         * has been scaled and offset according to the following
         * equation:
         *
         * @code{.cpp}
         *     physical_data = image_data * scale + offset;
         * @endcode
         *
         * The unit should conform to IAU Style Manual
         * recommendations.
         *
         * @see https://www.iau.org/publications/proceedings_rules/units/
         *
         * @return Unit of physical data in the source image.
         *
         * @note This value corresponds to the %FITS "BUNIT" keyword.

         */
        void bunit(std::string unit);

        /// Set the unit of physical data.
        std::string const & bunit() const { return this->bunit_; }

        /**
         * @brief Set the value for the map %FITS @c BZERO keyword.
         *
         * The default value of the %FITS @c BZERO keyword is zero.
         *
         * @param[in] zero @c BZERO keyword value.
         *
         * @note Setting this value will cause the data written to the
         *       %FITS file to be transformed according the equation:
         * @code{.cpp}
         *     (FITS value) = ((physical value) - BZERO) / BSCALE
         * @endcode
         */
        void bzero(std::optional<double> zero);

        /// Get the value for the map %FITS @c BZERO keyword.
        std::optional<double> bzero() const { return this->bzero_; }

        /// Set the %FITS @c DATAMAX value.
        void datamax(std::optional<double> max);

        /// Get the value for the map %FITS @c DATAMAX keyword.
        std::optional<double> datamax() const { return this->datamax_; }

        /// Set the %FITS @c DATAMIN value.
        void datamin(std::optional<double> min);

        /// Get the value for the map %FITS @c DATAMIN keyword.
        std::optional<double> datamin() const { return this->datamin_; }

        /// Set the %FITS @c EQUINOX value.
        void equinox(std::optional<double> e);

        /// Get the value for the map %FITS @c EQUINOX keyword.
        std::optional<double> equinox() const { return this->equinox_; }

                /// Set name of object being mapped.
        void instrument(std::string i);

        /// Get name of object being mapped.
        std::string const & instrument() const
        {
            return this->instrument_;
        }

        /// Set name of object being mapped.
        void object(std::string o);

        /// Get name of object being mapped.
        std::string const & object() const { return this->object_; }

        /// Set name of person who acquired the source data.
        void observer(std::string o);

        /// Get name of person who acquired the source data.
        std::string const & observer() const { return this->observer_; }

        /**
         * @brief Set organization or institution responsible for
         *        creating the map.
         */
        void origin(std::string o);

        /**
         * @brief Get organization or institution responsible for
         *        creating the map.
         *
         * @todo We may not want to automatically pull this value from
         *       the source %FITS file since that "origin" may not be
         *       the same as the one creating the map.
         */
        std::string const & origin() const { return this->origin_; }

        /**
         * @brief Set source data publication reference.
         *
         * @note The %FITS standard recommends that this be the
         *       19-digit bibliographic indentifier used in the
         *       Astrophysics Data System bibliographic databases or
         *       the Digital Object Identifier.
         *
         * @note This value corresponds to the %FITS "REFERENC"
         *       keyword.
         */
        void reference(std::string r);

        /// Get reference to where the source data was published.
        std::string const & reference() const { return this->reference_; }

        /**
         * @brief Set telescope used to acquire the source data.
         *
         * @note This value corresponds to the %FITS "TELESCOP"
         *       keyword.
         */
        void telescope(std::string t);

        /// Get telescope used to acquire the source data.
        std::string const & telescope() const { return this->telescope_; }

        /**
         * @brief Add map image comment.
         *
         * Push a map image comment to the comment list to be embedded
         * in the map image %FITS header data unit.  Each comment will
         * be placed in its own %FITS @c COMMENT.
         *
         * @param[in] comment Comment to be added to the map image
         *                    HDU.
         */
        void push_comment(comment_list_type::value_type comment);

        /// Get map image comments.
        comment_list_type const & comments() const
        {
            return this->comments_;
        }

        /**
         * @brief Add map image extension comment.
         *
         * Push a map image extension comment to the comment list to
         * be embedded in the map image extension %FITS header data
         * unit.  Each comment will be placed in its own %FITS
         * @c COMMENT.
         *
         * @note This set of %FITS image extension comments is
         *       only used for documenting the grid image generated by
         *       %MaRC.
         *
         * @param[in] comment Comment to be added to the map grid
         *                    image extension HDU.
         */
        void push_xcomment(comment_list_type::value_type comment);

        /// Get map image extension comments.
        comment_list_type const & xcomments() const
        {
            return this->xcomments_;
        }

        /**
         * @brief Merge map paremeters.
         *
         * Merge the given set of map parameters, @a p, with this
         * one.  Map parameters in this object will generally override
         * those in the map parameters @a p.  The goal is to provide a
         * simple way for user supplied map parameters to override
         * those that are automatically populated from the image (map
         * plane) factories.
         *
         * @param[in,out] p Map parameters to be merged.  @a p may be
         *                  modified during the merge, e.g. via
         *                  @c std::move().
         *
         * @return @c true if the map parameter merge was successful,
         *         and @c false otherwise.
         */
        bool merge(map_parameters & p);

    private:

        bool merge_optional(char const * key,
                            std::optional<double> & to,
                            std::optional<double> & from);

        bool merge_optional(char const * key,
                            MaRC::blank_type & to,
                            MaRC::blank_type & from);

        bool merge_optional(char const * key,
                            std::string & to,
                            std::string from);

    private:

        /**
         *
         */
        std::set<std::string> locked_keywords_;

        /**
         *
         */
        bool user_supplied_;

        /// Map plane to which these parameters belong.
        int const plane_;

        /**
         * @brief Source data author.
         *
         * The person that compiled the data in the map.
         *
         * @note This value corresponds to the %FITS "AUTHOR" keyword
         */
        std::string author_;

        /**
         * @brief Bits per pixel.
         *
         * %FITS bits per pixel (i.e. 8, 16, 32, 64, -32, or -64).
         *
         * @note This value corresponds to the %FITS "BITPIX"
         *       keyword.
         */
        int bitpix_;

        /**
         * @brief  Value of pixels with undefined physical value.
         *
         * @note This value is only valid for integer typed maps.
         * @note This value corresponds to the %FITS "BLANK" keyword.
         */
        blank_type blank_;

        /**
         * @brief Coefficient of linear term in the scaling equation.
         *
         * @note This value corresponds to the %FITS "BSCALE" keyword.
         */
        std::optional<double> bscale_;

        /**
         * @brief Unit of physical data.
         *
         * Get the units of the physical data, i.e. image data that
         * has been scaled and offset according to the following
         * equation:
         *
         * @code{.cpp}
         *     physical_data = image_data * scale + offset;
         * @endcode
         *
         * The unit should conform to IAU Style Manual
         * recommendations.
         *
         * @see https://www.iau.org/publications/proceedings_rules/units/
         *
         * @return Unit of physical data in the source image.
         *
         * @note This value corresponds to the %FITS "BUNIT" keyword.
         */
        std::string bunit_;

        /**
         * @brief Physical value corresponding to an array value of
         *        zero.
         *
         * @note This value corresponds to the %FITS "BZERO" keyword.
         */
        std::optional<double> bzero_;

        /**
         * @brief Maximum valid physical value.
         *
         * @note This value corresponds to the %FITS "DATAMAX"
         *       keyword.
         *
         * @bug On platforms that implement the IEEE 754 floating
         *      point standard, the use of @c double as the underlying
         *      @c DATAMAX type will cause loss of precision if the
         *      %FITS @c DATAMAX values is an integer that requires
         *      more than 53 bits since the significand of a 64 bit
         *      IEEE 754 floating point value is only 53 bits wide.
         *      Special handling will be need to be implemented to
         *      handle such a case.
         */
        std::optional<double> datamax_;

        /**
         * @brief Minimum valid physical value.
         *
         * @note This value corresponds to the %FITS "DATAMIN"
         *       keyword.
         *
         * @bug On platforms that implement the IEEE 754 floating
         *      point standard, the use of @c double as the underlying
         *      @c DATAMIN type will cause loss of precision if the
         *      %FITS @c DATAMIN values is an integer that requires
         *      more than 53 bits since the significand of a 64 bit
         *      IEEE 754 floating point value is only 53 bits wide.
         *      Special handling will be need to be implemented to
         *      handle such a case.
         */
        std::optional<double> datamin_;

        /**
         * @brief Epoch of the mean equator and equinox in years.
         *
         * @note This value must be non-negative.
         * @note This value corresponds to the %FITS "EQUINOX"
         *       keyword.
         */
        std::optional<double> equinox_;

        /**
         * @brief Instrument used to acquire the data.
         *
         * @note This value corresponds to the %FITS "INSTRUME"
         *       keyword.
         */
        std::string instrument_;

        /**
         * @brief Name of observed object.
         *
         * @note This value corresponds to the %FITS "OBJECT"
         *       keyword.
         */
        std::string object_;

        /**
         * @brief Name of person who acquired the source data.
         *
         * @note This value corresponds to the %FITS "OBSERVER"
         *       keyword.
         */
        std::string observer_;

        /**
         * @brief Organization or institution responsible for creating
         *        the map %FITS file.
         *
         * @note This value corresponds to the %FITS "ORIGIN" keyword.
         */
        std::string origin_;

        /**
         * @brief Reference to where the source data was published.
         *
         * @note The %FITS standard recommends that this be the
         *       19-digit bibliographic indentifier used in the
         *       Astrophysics Data System bibliographic databases or
         *       the Digital Object Identifier.
         *
         * @note This value corresponds to the %FITS "REFERENC"
         *       keyword.
         */
        std::string reference_;

        /**
         * @brief Telescope used to acquire the source data.
         *
         * @note This value corresponds to the %FITS "TELESCOP"
         *       keyword.
         */
        std::string telescope_;

        /**
         * @brief User supplied map comments.
         *
         * The user may provide map comments to document various
         * aspects of the data being mapped, and/or the map itself.
         *
         * @note The comments correspond to the %FITS "COMMENT"
         *       keyword.
         */
        comment_list_type comments_;

        /**
         * @brief User supplied map grid comments.
         *
         * The user may provide map comments to document various
         * aspects of the grid being mapped.
         *
         * @note The comments correspond to the %FITS "COMMENT"
         *       keyword in the "GRID" %FITS image extension.
         */
        comment_list_type xcomments_;

        /**
         * @brief Map documentation created by %MaRC.
         *
         * In some cases %MaRC will document some aspects of the map
         * it has decided to use without user involvement in history
         * comments.
         *
         * @note The history comments correspond to the %FITS
         *       "HISTORY" keyword.
         */
        comment_list_type histories_;

    };
}


#endif  /* MARC_MAP_PARAMETERS_H */
