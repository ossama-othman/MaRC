// -*- C++ -*-

//==========================================================================
/**
 *  @file ImageFactory.h
 *
 *  $Id: ImageFactory.h,v 1.1 2004/07/03 10:59:42 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================

#ifndef MARC_IMAGE_FACTORY_H
#define MARC_IMAGE_FACTORY_H

#include "MaRC/ValuePtr.h"

namespace MaRC
{
  class SourceImage;

  /**
   * @class ImageFactory
   *
   * @brief Abstract factory class containing interface for image
   *        factories.
   *
   * @note An @c ImageFactory can be considered a map plane factory as
   *       well.
   */
  class ImageFactory
  {
  public:

    /// Constructor.
    ImageFactory (void);

    /// Destructor.
    virtual ~ImageFactory (void);

    /// Create an Image.
    virtual SourceImage * make (void) = 0;

    /// Clone operation that polymorphically copies the concrete
    /// @c ImageFactory object.
    /**
     * This clone operation is generally used in conjunction with the
     * VP_traits<ImageFactory> template specialization to prevent the
     * slicing that occurs when copying through a copy constructor
     * instead.
     */
    virtual ImageFactory * clone (void) const = 0;

    /// Set minimum allowed data value in map plane.  (data > minimum)
    void minimum (double m) { this->minimum_ = m; }

    /// Set maximum allowed data value in map plane.  (data < maximum)
    void maximum (double m) { this->maximum_ = m; }

    /// Return minimum allowed data value in map plane.  (data > minimum)
    double minimum (void) const { return this->minimum_; }

    /// Return maximum allowed data value in map plane.  (data < maximum)
    double maximum (void) const { return this->maximum_; }

  private:

    /// Minimum allowed data value in map plane.  (data > minimum)
    double minimum_;

    /// Maximum allowed data value in map plane.  (data < maximum)
    double maximum_;

  };

  // ImageFactory specialization of the ValuePtr traits template.
  template<>
  struct VP_traits<ImageFactory>
  {
    static ImageFactory * clone (const ImageFactory * p)
    {
      return p->clone ();
    }
  };

}

#endif  /* MARC_IMAGE_FACTORY_H */
