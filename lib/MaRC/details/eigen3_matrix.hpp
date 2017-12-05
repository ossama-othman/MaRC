// -*- C++ -*-
/**
 * @file marc_matrix.hpp
 *
 * Eigen linear algebra library based mathematical matrix operations.
 *
 * Copyright (C) 2004, 2017  Ossama Othman
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

#ifndef MARC_DETAILS_EIGEN3_MATRIX_HPP
#define MARC_DETAILS_EIGEN3_MATRIX_HPP

#include <Eigen/Dense>

namespace MaRC
{
    /**
     * @typedef Convenience matrix type
     *
     * @c Matrix type used heavily through out the MaRC library.
     */
    using DMatrix = Eigen::Matrix3d;

    // ---------------------------------------------------------

    /// Matrix transpose.
    inline DMatrix transpose(DMatrix const & m)
    {
        return m.transpose();
    }
}

#endif  /* MARC_DETAILS_EIGEN3_MATRIX_HPP */
