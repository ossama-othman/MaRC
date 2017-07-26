// -*- C++ -*-

//==========================================================================
/**
 * @file Image.h
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
//==========================================================================


#ifndef MARC_IMAGE_H
#define MARC_IMAGE_H

#include <MaRC/NaN.h>

#include <limits>
#include <cassert>


namespace MaRC
{
  /**
   * @struct Image_traits
   *
   * @brief @c Image traits template structure.
   *
   * The @c Image template class delegates some operations to this
   * template traits structure.
   */
  template <typename T>
  struct Image_traits
  {
    /// Initialize the underlying image array.
    /**
     * @param begin Iterator corresponding to beginning of image array.
     * @param end   Iterator corresponding to end of image array.
     */
    static void initialize_image (T * const begin, T const * const end)
    {
      for (T * image = begin; image != end; ++image)
        *image = T ();
    }
  };

  // Image_traits specializations.
  template <>
  struct Image_traits<float>
  {
    static void initialize_image (float * const begin,
                                  float const * const end)
    {
      for (float * image = begin; image != end; ++image)
        *image = static_cast<float> (MaRC::NaN);
    }
  };

  template <>
  struct Image_traits<double>
  {
    static void initialize_image (double * const begin,
                                  double const * const end)
    {
      for (double * image = begin; image != end; ++image)
        *image = static_cast<double> (MaRC::NaN);
    }
  };


  // --------------------------------------------------------

  /**
   * @class Image
   *
   * @brief Encapsulation of two dimensional image array.
   *
   * This class merely encapsulates a two dimensional image, and
   * exposes a simple interface that includes data element access
   * operators.  It also eases memory management of the underlying
   * data array.
   */
  template <typename T>
  class Image
  {
  public:

    typedef T data_type;

    /// Constructor.
    /**
     * @param image      Image array to be managed by this Image
     *                   object.
     * @param samples    The width  in pixels of the image.
     * @param lines      The height in pixels of the image.
     */
    Image (T * image, unsigned int samples, unsigned int lines);


    /// Constructor.
    /**
     * This constructor allocates and initializes an array of type T.
     *
     * @param samples    The width  in pixels of the image.
     * @param lines      The height in pixels of the image.
     */
    Image (unsigned int samples, unsigned int lines);

    /// Copy constructor.
    Image (const Image<T> & other);

    /// Assignment operator.
    Image<T> & operator= (const Image<T> & other);

    /// Destructor.
    ~Image (void);

    /// Return the number of samples in the image projection.
    unsigned int samples (void) const { return this->samples_; }

    /// Return the number of lines in the image projection.
    unsigned int lines (void) const { return this->lines_; }

    /// Data access operator.
    /**
     * @param index Array element in underlying data array.
     *
     * @return Mutable reference to desired data element in underlying
     *         array.
     */
    T & operator[] (unsigned int index)
    {
#ifdef DEBUG
      assert (index < this->samples_ * this->lines_);
#endif

      return this->image_[index];
    }

    /// const data access operator.
    /**
     * @param index Array element in underlying data array.
     *
     * @return Mutable reference to desired data element in underlying
     *         array.
     */
    T const & operator[] (unsigned int index) const
    {
#ifdef DEBUG
      assert (index < this->samples_ * this->lines_);
#endif

      return this->image_[index];
    }


    /// Convenience data access operator
    /**
     * @param sample Sample value in underlying data array.
     * @param line   Line   value in underlying data array.
     *
     * @return Mutable reference to desired data element in underlying
     *         array.
     */
    T & operator() (unsigned int sample, unsigned int line)
    {
      const unsigned int index = line * this->samples_ + sample;

#ifdef DEBUG
      assert (index < this->samples_ * this->lines_);
#endif

      return this->image_[index];
    }

    /// Return pointer to the underlying data array.
    /**
     * @note Image retains ownership of the memory.
     *
     * @return Pointer to the underlying data array.
     */
    T * get (void) const throw () { return this->image_; }

    /// Equality operator.
    bool operator== (const Image<T> & rhs) const;

  private:

    /// The number of samples in the image projection.
    unsigned int samples_;

    /// The number of lines in the image projection.
    unsigned int lines_;

    /// Underlying image array.
    T * image_;

  };

}

#include "MaRC/Image.cpp"

#endif
