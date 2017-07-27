//   -*- C++ -*-
/**
 * @file NullPhotometricCorrection.h
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
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

#ifndef MARC_NULL_PHOTOMETRIC_CORRECTION_H
#define MARC_NULL_PHOTOMETRIC_CORRECTION_H

#include <MaRC/PhotometricCorrection.h>


namespace MaRC
{
    class BodyData;

    /**
     * @class NullPhotometricCorrection
     *
     * @brief Null (no-op) photometric correction strategy.
     *
     * This photometric correction strategy is a no-op.  It performs
     * no photometric correction.
     */
    class NullPhotometricCorrection : public PhotometricCorrection
    {
    public:

        /// Destructor
        virtual ~NullPhotometricCorrection (void);

        /**
         * @name PhotometricCorrection Methods
         *
         * Methods required by the @c PhotometricCorrection abstract
         * base class.
         *
         * @see @c PhotometricCorrection::correct()
         */
        //@{
        /// Perform photometric correction.
        /**
         * This particular implementation is a no-op.
         */
        virtual int correct(BodyData const & body,
                            double sub_observ_lat,
                            double sub_observ_lon,
                            double sub_solar_lat,
                            double sub_solar_lon,
                            double lat,
                            double lon,
                            double range,
                            double & data);
        //@}

  };

}


#endif  /* MARC_NULL_PHOTOMETRIC_CORRECTION_H */
