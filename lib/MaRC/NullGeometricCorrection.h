//  -*- C++ -*-
//
// $Id: NullGeometricCorrection.h,v 1.3 2004/07/06 00:01:41 othman Exp $

#ifndef MARC_NULL_GEOMETRIC_CORRECTION_H
#define MARC_NULL_GEOMETRIC_CORRECTION_H


#include <MaRC/GeometricCorrection.h>

namespace MaRC
{
  /**
   * @class NullGeometricCorrection
   *
   * @brief Null (no-op) geometric correction strategy.
   *
   * This geometric correction strategy is a no-op.  It performs no
   * geometric correction.
   */
  class NullGeometricCorrection : public GeometricCorrection
  {
  public:

    /**
     * @name GeometricCorrection Methods
     *
     * Virtual methods required by the GeometricCorrection abstract
     * base class.
     */
    //@{
    virtual void image_to_object (double & x, double & z);
    virtual void object_to_image (double & x, double & z);
    virtual GeometricCorrection * clone (void) const;
    //@}

  };

}

#endif  /* MARC_NULL_GEOMETRIC_CORRECTION_H */
