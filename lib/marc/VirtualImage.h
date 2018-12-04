// -*- C++ -*-
/**
 * @file VirtualImage.h
 *
 * Copyright (C) 2003-2004, 2017-2018  Ossama Othman
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

#ifndef MARC_VIRTUAL_IMAGE_H
#define MARC_VIRTUAL_IMAGE_H

#include "marc/SourceImage.h"
#include <marc/Export.h>


namespace MaRC
{

    /**
     * @class VirtualImage VirtualImage.h <marc/VirtualImage.h>
     *
     * @brief Base class for virtual source images.
     *
     * Data from virtual images are computed at run-time rather than
     * retrieved from static sources such as images stored on a
     * filesystem.
     */
    class MARC_API VirtualImage : public SourceImage
    {
    public:

        /**
         * @brief Constructor.
         *
         * @param[in] s Linear scaling coefficient applied to computed
         *              data.
         * @param[in] o Linear offset value applied to all (scaled)
         *              computed data.
         */
        VirtualImage(double s = 1, double o = 0);

        /// Destructor.
        virtual ~VirtualImage() = default;

        /**
         * @brief Retrieve data from virtual image.
         *
         * Retrieve data from virtual image and apply configured data
         * transformations, if any.  Raw physical data is
         * computed/retrieved from the @c read_data_i() template
         * method.
         *
         * @param[in]  lat  Planetocentric latitude in radians.
         * @param[in]  lon  Longitude in radians.
         * @param[out] data Data retrieved from image.
         *
         * @retval true  Data retrieved
         * @retval false No data retrieved.
         *
         * @note The retrieved data will be scaled and offset using
         *       the respective values passed to the @c VirtualImage
         *       constructor.  This behavior differs from behavior
         *       documented in the @c SourceImage abstract base class
         *       where the data is expected to be physical data.  The
         *       reason for this is historical.  %MaRC virtual images,
         *       such as &mu;, &mu;<SUB>0</SUB> and cos(&phi;), were
         *       originally embedded in %FITS files with potentially
         *       different kinds of physical data stored in an integer
         *       typed map "cube".  To retain significant digits,
         *       floating point physical data values, such as the
         *       computed cosines mentioned earlier, were scaled and
         *       offset accordingly.  Ideally, different physical data
         *       types shouldn't be mixed and matched in such a data
         *       cube, and should instead be stored in a cube
         *       containing matching physical data types within a
         *       map.  Appropriate scale and offset values could then
         *       be applied to the data cube as a whole, rather than
         *       having to worry about applying a different scale and
         *       offset for each cube "plane".
         *
         * @see read_data_i()
         */
        virtual bool read_data(double lat,
                               double lon,
                               double & data) const override;

        /**
         * @name Linear Data Transformation
         *
         * These methods return the scale and offset values that
         * should be used when linearly transforming data read from a
         * @ c VirtualImage and plotted on a map to the true physical
         * values.  In particular, data should be transformed
         * according to the following equation:
         *
         * @code
         *     physical_value = map_value * scale() + offset();
         * @endcode
         *
         * The default implementation returns values that causes no
         * transformation to occur.
         */
        //@{
        /**
         * @brief Data scale.
         *
         * Linear scaling coefficient that should be applied to map
         * data to transform that data to true physical data.
         *
         * @return Data scale.
         */
        double scale() const { return 1 / this->scale_; }

        /**
         * @brief Data offset
         * Offset value that should be applied to all (scaled)
         * computed data.  This value corresponds to zero in the
         * virtual image.
         *
         * @return Data offset.
         */
        double offset() const { return this->scale() * -this->offset_; }
        //@}

    private:

        /**
         * @brief Compute physical data specific to a given virtual
         *        image.
         *
         * This template method is the core implementation of the
         * @c read_data() method.
         *
         * @param[in]  lat  Planetocentric latitude in radians.
         * @param[in]  lon  Longitude in radians.
         * @param[out] data Physical data retrieved from image.
         *
         * @retval true  Physical data retrieved
         * @retval false No physical data retrieved.
         *
         * @see read_data()
         */
        virtual bool read_data_i(double lat,
                                 double lon,
                                 double & data) const = 0;

    private:

        /**
         * @brief Linear scaling coefficient applied to physical
         *        data.
         *
         * Linear scaling coefficient applied to physical data to
         * allow data to fit in map array element of specific type
         * with the most amount of significant digits.
         *
         * @note This is inverse of the scaling coefficient needed to
         *       retrieve the true physical value.
         *
         * @see @c VirtualImage::scale()
         */
        double const scale_;

        /**
         * @brief Offset value applied to scaled physical data.
         *
         * This offset is applied to scaled physical data to allow
         * data fit in a map array element of specific type with the
         * most amount of significant digits.
         *
         * @note This is the negative offset multiplied by the inverse
         *       of the of scaling value needed to retrieve the true
         *       physical value.
         *
         * @see @c VirtualImage::offset()
         */
        double const offset_;

    };

} // End MaRC namespace


#endif  /* MARC_VIRTUAL_IMAGE_H */
