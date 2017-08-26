//   -*- C++ -*-
/**
 * @file Geometry.h
 *
 * MaRC geometry related functions
 *
 * Copyright (C) 1999, 2003-2004, 2017  Ossama Othman
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

#ifndef MARC_GEOMETRY_H
#define MARC_GEOMETRY_H

#include <MaRC/Matrix.h>

#include <cstddef>


namespace MaRC
{

    /**
     * @name Convenience Vector and Matrix Types
     *
     * @c Vector and @c Matrix types used heavily through out the MaRC
     * library.
     */
    //@{
    typedef Vector<double, 3> DVector;
    typedef Matrix<double, 3, 3> DMatrix;
    //@}

    /**
     * @namespace Geometry
     *
     * @brief Geometry related functions.
     */
    namespace Geometry
    {
        /**
         * @name Coordinate System Rotations
         *
         * @brief Set functions that perform or generate coordinate
         *        system rotations.
         *
         * These functions either perform or generate @em coordinate
         * @em system @em rotations, as opposed to rotations in a
         * fixed coordinate system where the rotation matrix would be
         * the transpose of the coordinate system rotation matrix.
         *
         * All coordinate transformations assume a right-handed
         * coordinate system, meaning a positive angle causes
         * counterclockwise rotation to occur about a given axis.
         *
         * All angles are in radians.
         */
        //@{
        /// Rotate vector about x-axis.
        /**
         * @param[in]  angle Angle about the x-axis through which the
         *                   coordinate system will be rotated.
         * @param[in]  v     Vector in the original coordinate
         *                   system.
         * @param[out] r     Vector in the rotated coordinate system.
         */
        void RotX(double angle, DVector const & v, DVector & r);

        /// Rotate about y-axis.
        /**
         * @param[in]  angle Angle about the y-axis through which the
         *                   coordinate system will be rotated.
         * @param[in]  v     Vector in the original coordinate
         *                   system.
         * @param[out] r     Vector in the rotated coordinate system.
         */
        void RotY(double angle, DVector const & v, DVector & r);

        /// Rotate about z-axis.
        /**
         * @param[in]  angle Angle about the z-axis through which the
         *                   coordinate system will be rotated.
         * @param[in]  v     Vector in the original coordinate
         *                   system.
         * @param[out] r     Vector in the rotated coordinate system.
         */
        void RotZ(double angle, DVector const & v, DVector & r);

        /// Return a transformation matrix that rotates a coordinate
        /// system about the x-axis.
        /**
         * @param[in] angle Angle about the x-axis through which the
         *                  returned rotation matrix will rotate a
         *                  coordinate system.
         *
         * @return Matrix that will rotate a coordinate system
         *         @a angle radians about the x-axis.
         */
        DMatrix RotXMatrix(double angle);

        /// Return a transformation matrix that rotates a coordinate
        /// system about the y-axis.
        /**
         * @param[in] angle Angle about the y-axis through which the
         *                  returned rotation matrix will rotate a
         *                  coordinate system.
         *
         * @return Matrix that will rotate a coordinate system
         *         @a angle radians about the y-axis.
         */
        DMatrix RotYMatrix(double angle);

        /// Return a transformation matrix that rotates a coordinate
        /// system about the z-axis.
        /**
         * @param[in] angle Angle about the z-axis through which the
         *                  returned rotation matrix will rotate a
         *                  coordinate system.
         *
         * @return Matrix that will rotate a coordinate system
         *         @a angle radians about the z-axis.
         */
        DMatrix RotZMatrix(double angle);
        //@}

    }
}


#endif  /* MARC_GEOMETRY_H */
