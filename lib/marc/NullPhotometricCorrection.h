//   -*- C++ -*-
/**
 * @file NullPhotometricCorrection.h
 *
 * Copyright (C) 2003-2004, 2017  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_NULL_PHOTOMETRIC_CORRECTION_H
#define MARC_NULL_PHOTOMETRIC_CORRECTION_H

#include <marc/PhotometricCorrection.h>


namespace MaRC
{

    /**
     * @class NullPhotometricCorrection NullPhotometricCorrection.h <marc/NullPhotometricCorrection.h>
     *
     * @brief Null (no-op) photometric correction strategy.
     *
     * This photometric correction strategy is a no-op.  It performs
     * no photometric correction.
     */
    class MARC_API NullPhotometricCorrection final
        : public PhotometricCorrection
    {
    public:

        /// Destructor
        ~NullPhotometricCorrection() override = default;

        /**
         * @name PhotometricCorrection Methods
         *
         * Methods required by the @c PhotometricCorrection abstract
         * base class.
         *
         * @see @c PhotometricCorrection::correct()
         */
        ///@{
        /**
         * @brief Perform photometric correction.
         *
         * This particular implementation is a no-op.
         *
         * @return @c true in all cases.
         */
        bool correct(ViewingGeometry const & geometry,
                     double & data) override;
        ///@}

    };

}


#endif  /* MARC_NULL_PHOTOMETRIC_CORRECTION_H */
