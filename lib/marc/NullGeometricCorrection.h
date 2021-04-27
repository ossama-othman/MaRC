//  -*- C++ -*-
/**
 * @file NullGeometricCorrection.h
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
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
        ///@{
        void image_to_object(double & line,
                             double & sample) const override;
        void object_to_image(double & line,
                             double & sample) const override;
      ///@}

    };

}


#endif  /* MARC_NULL_GEOMETRIC_CORRECTION_H */
