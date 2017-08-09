// -*- C++ -*-

//==========================================================================
/**
 *  @file GeometricCorrection.h
 *
 *  $Id: GeometricCorrection.h,v 1.2 2004/07/06 00:01:41 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================

#ifndef MARC_GEOMETRIC_CORRECTION_H
#define MARC_GEOMETRIC_CORRECTION_H

#include "MaRC/ValuePtr.h"

namespace MaRC
{
  /**
   * @class GeometricCorrection
   *
   * @brief Abstract base class for all geometric correction
   *        strategies.
   *
   * Concrete geometric correction classes must implement the interface
   * required by this abstract base class.
   *
   * Please note: An 'x' coordinate is a sample coordinate relative to
   * the image optical axis; a 'z' coordinate is a line coordinate
   * relative to the image optical axis with value increasing from bottom
   * to top.
   */
  class GeometricCorrection
  {
  public:

    /// Destructor.
    virtual ~GeometricCorrection (void);

    /// Convert from image space to object space.
    /**
     * Apply lens aberration correction.
     *
     * @param x   Image-space x coordinate converted to object space.
     * @param z   Image-space z coordinate converted to object space.
     */
    virtual void image_to_object (double & x, double & z) = 0;

    /// Convert from object space to image space.
    /**
     * Introduce lens aberration to determine actual image space pixel
     * values.
     *
     * @param x   Object-space x coordinate converted to image space.
     * @param z   Object-space z coordinate converted to image space.
     */
    virtual void object_to_image (double & x, double & z) = 0;


    /// Clone operation that polymorphically copies the concrete
    /// @c GeometricCorrection object.
    /**
     * This clone operation is generally used in conjunction with the
     * VP_traits<GeometricCorrection> template specialization to
     * prevent the slicing that occurs when copying through a copy
     * constructor instead.
     */
    virtual GeometricCorrection * clone (void) const = 0;

  };

  // GeometricCorrection specialization of the ValuePtr traits template.
  template<>
  struct VP_traits<GeometricCorrection>
  {
    static GeometricCorrection * clone (const GeometricCorrection * p)
    {
      return p->clone ();
    }
  };


} // End MaRC namespace

#endif  /* MARC_GEOMETRIC_CORRECTION_H */
