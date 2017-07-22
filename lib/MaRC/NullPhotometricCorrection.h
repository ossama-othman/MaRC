//   -*- C++ -*-

//==========================================================================
/**
 *  @file NullPhotometricCorrection.h
 *
 *  @author Ossama Othman
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
                         double sub_observ_lat,
                         double sub_observ_lon,
                         double sub_solar_lat,
                         double sub_solar_lon,
                         double lat,
                         double lon,
                         double range,
                         double & data);

    /// Clone this @c NullPhotometricCorrection object.
    virtual PhotometricCorrection * clone (void) const;
    //@}

  };

}


#endif  /* MARC_NULL_PHOTOMETRIC_CORRECTION_H */
