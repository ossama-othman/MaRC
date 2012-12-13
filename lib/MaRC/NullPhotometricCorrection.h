//   -*- C++ -*-

//==========================================================================
/**
 *  @file NullPhotometricCorrection.h
 *
 *  $Id: NullPhotometricCorrection.h,v 1.3 2004/07/06 00:01:41 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================

#ifndef MARC_NULL_PHOTOMETRIC_CORRECTION_H
#define MARC_NULL_PHOTOMETRIC_CORRECTION_H

#include <MaRC/PhotometricCorrection.h>

namespace MaRC
{
  class BodyData;

  /**
   * @class NullPhotometricCorrection
   *
   * @brief Null (no-op) photometric correction strategy.
   *
   * This photometric correction strategy is a no-op.  It performs no
   * photometric correction.
   */
  class NullPhotometricCorrection : public PhotometricCorrection
  {
  public:

    /// Destructor
    virtual ~NullPhotometricCorrection (void);

    /**
     * @name PhotometricCorrection Methods
     *
     * Methods required by the @c PhotometricCorrection abstract base
     * class.
     *
     * @see @c PhotometricCorrection::correct()
     */
    //@{
    /// Perform photometric correction.
    /**
     * This particular implementation is a no-op.
     */
    virtual int correct (const BodyData & body,
                         const double & sub_observ_lat,
                         const double & sub_observ_lon,
                         const double & sub_solar_lat,
                         const double & sub_solar_lon,
                         const double & lat,
                         const double & lon,
                         const double & range,
                         double & data);

    /// Clone this @c NullPhotometricCorrection object.
    virtual PhotometricCorrection * clone (void) const;
    //@}

  };

}


#endif  /* MARC_NULL_PHOTOMETRIC_CORRECTION_H */
