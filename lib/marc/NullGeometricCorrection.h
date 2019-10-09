//  -*- C++ -*-
/**
 * @file NullGeometricCorrection.h
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

#ifndef MARC_NULL_GEOMETRIC_CORRECTION_H
#define MARC_NULL_GEOMETRIC_CORRECTION_H

#include <marc/GeometricCorrection.h>
#include <marc/Export.h>


namespace MaRC
{
    /**
     * @class NullGeometricCorrection NullGeometricCorrection.h <marc/NullGeometricCorrection.h>
     *
     * @brief Null (no-op) geometric correction strategy.
     *
     * This geometric correction strategy is a no-op.  It performs no
     * geometric correction.
     */
    class MARC_API NullGeometricCorrection final
        : public GeometricCorrection
    {
    public:

        NullGeometricCorrection() = default;
        ~NullGeometricCorrection() override = default;

        /**
         * @name GeometricCorrection Methods
         *
         * Virtual methods required by the GeometricCorrection
         * abstract base class.
         */
        //@{
        void image_to_object(double & line,
                             double & sample) const override;
        void object_to_image(double & line,
                             double & sample) const override;
      //@}

    };

}


#endif  /* MARC_NULL_GEOMETRIC_CORRECTION_H */
