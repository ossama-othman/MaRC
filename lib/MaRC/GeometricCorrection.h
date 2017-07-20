// -*- C++ -*-

//==========================================================================
/**
 *  @file GeometricCorrection.h
 *
 *  @author Ossama Othman
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
     * @param line   Image-space line coordinate converted to object space.
     * @param sample Image-space sample coordinate converted to object space.
     */
    virtual void image_to_object (double & line, double & sample) = 0;

    /// Convert from object space to image space.
    /**
     * Introduce lens aberration to determine actual image space pixel
     * values.
     *
     * @param line   Object-space line coordinate converted to image space.
     * @param sample Object-space sample coordinate converted to image space.
     */
    virtual void object_to_image (double & line, double & sample) = 0;


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
