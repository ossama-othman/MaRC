//  -*- C++ -*-

//==========================================================================
/**
 *  @file GLLGeometricCorrection.h
 *
 *  @author Ossama Othman
 */
//==========================================================================


#ifndef MARC_GLL_GEOMETRIC_CORRECTION_H
#define MARC_GLL_GEOMETRIC_CORRECTION_H


#include <MaRC/GeometricCorrection.h>


//   // Kludge to "automatically" detect summation mode images
//   if (Samples_ > OA_SAMP)
//     this->summation_mode_ = 1; // Full frame
//   else
//     this->summation_mode_ = 2; // Summation mode frame

namespace MaRC
{
  /**
   * @class GLLGeometricCorrection
   *
   * @brief Galileo Spacecraft lens aberration correction strategy.
   *
   * Galileo specific concrete geometric correction strategy.
   */
  class GLLGeometricCorrection : public GeometricCorrection
  {
  public:

    /// Constructor
    /**
     * @param samples Number of samples in the PhotoImage.  Only used
     *                to determine whether summation mode should be
     *                enabled.
     */
    GLLGeometricCorrection (unsigned int samples);

    /**
     * @name GeometricCorrection Methods
     *
     * Virtual methods required by the GeometricCorrection abstract
     * base class.
     */
    //@{
    virtual void image_to_object (double & line, double & sample);
    virtual void object_to_image (double & line, double & sample);
    virtual GeometricCorrection * clone (void) const;
    //@}

    /// Return current summation mode
    bool summation_mode (void) const { return this->summation_mode_; }

  private:

    /// Compute the cubic root of a given value
    static double cube_root (const double & x);

  private:

    /// true = summation mode, false = full frame
    const bool summation_mode_;

  };

}

#endif  /* MARC_GLL_GEOMETRIC_CORRECTION_H */
