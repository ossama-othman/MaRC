/**
 * @file eigen3_vector.hpp
 *
 * Eigen linear algebra library based mathematical vector operations.
 *
 * Copyright (C) 2017  Ossama Othman
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

#ifndef MARC_DETAILS_EIGEN3_VECTOR_HPP
#define MARC_DETAILS_EIGEN3_VECTOR_HPP

#include <Eigen/Dense>


namespace MaRC
{
    /**
     * @typedef Convenience vector type
     *
     * @c Vector type used heavily through out the MaRC library.
     */
    using DVector = Eigen::Vector3d;

    /// Obtain magnitude of vector with three rows.
    /**
     * @param[in] v Vector for which the magnitude will be
     *            calculated.
     *
     * @return Magnitude of vector @a v.
     */
    inline auto magnitude(DVector const & v)
    {
        return v.norm();
    }

    /// Convert a vector to a unit vector.
    /**
     * @param[in,out] v Vector to convert to a unit vector.
     *
     * @attention This function requires that the vector @a v contain
     *            floating point values since it is not possible to
     *            store fractional values in an integer.
     */
    inline void to_unit_vector(DVector & v)
    {
        auto const mag = MaRC::magnitude(v);

        v /= mag;
    }

    /// Obtain dot product of two vectors.
    /**
     * Given two vector @a and @a b, calculate their dot product.
     *
     * @param[in] a First  vector operand.
     * @param[in] b Second vector operand.
     *
     * @return Dot product of vectors @a a and @a b.
     */
    inline auto dot_product(DVector const & a,
                            DVector const & b)
    {
        return a.dot(b);
    }
}


#endif  /* MARC_DETAILS_EIGEN3_VECTOR_HPP */
