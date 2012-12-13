//   -*- C++ -*-

//==========================================================================
/**
 *  @file BodyData.h
 *
 *  $Id: BodyData.h,v 1.3 2004/10/13 09:53:35 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================

#ifndef MARC_BODY_DATA_H
#define MARC_BODY_DATA_H

#include "MaRC/ValuePtr.h"


namespace MaRC
{

  /**
   * @class BodyData
   *
   * @brief Class representing the body under observation.
   *
   * Concrete @c BodyData implementations must subclass this abstract
   * base class.
   */
  class BodyData
  {
  public:

    /// Constructor
    /**
     * @param prograde Flag that states whether body rotation is
     *                 prograde or retrograde.
     */
    BodyData (bool prograde);

    /// Destructor
    virtual ~BodyData (void);

    /// Clone operation that polymorphically copies the concrete
    /// @c BodyData object.
    /**
     * This clone operation is generally used in conjunction with the
     * VP_traits<BodyData> template specialization to prevent the
     * slicing that occurs when copying through a copy constructor
     * instead.
     */
    virtual BodyData * clone (void) const = 0;

    /// Return the direction of rotation.
    /**
     * @return @c true if the current rotation is prograde.
     */
    bool prograde (void) const { return this->prograde_; }

    /// Find radius as a function of centric latitude.
    /**
     * @param lat Bodycentric (e.g. planetocentric) latitude.
     *
     * @return Radius at given latitude.
     */
    virtual double centric_radius (const double & lat) const = 0;

    /// Convert from GRAPHIC to CENTRIC latitude
    /**
     * @param lat graphic (e.g. planetographic) latitude
     *
     * @return Bodycentric latitude.
     */
    virtual double centric_latitude (const double & lat) const = 0;

    /// Convert from CENTRIC to GRAPHIC latitude
    /**
     * @param lat centric (e.g. planetocentric) latitude
     *
     * @return Bodygraphic latitude.
     */
    virtual double graphic_latitude (const double & lat) const = 0;

    /// Return cosine of emission angle
    /**
     * @param sub_observ_lat Bodycentric subobservation latitude
     * @param sub_observ_lon Subobservation longitude
     * @param lat            Bodycentric latitude
     * @param lon            Longitude
     * @param range          Observer range to subobservation point
     *
     * @return Cosine of emission angle.
     */
    virtual double mu (const double & sub_observ_lat,
                       const double & sub_observ_lon,
                       const double & lat,
                       const double & lon,
                       const double & range) const = 0;

    /// Return cosine of incidence angle
    /**
     * @param sub_solar_lat Bodycentric subsolar latitude
     * @param sub_solar_lon Subsolar longitude
     * @param lat           Bodycentric latitude
     * @param lon           Longitude
     *
     * @return Cosine of incidence angle
     *
     * @note Sun is assumed to be an infinite distance away.
     */
    virtual double mu0 (const double & sub_solar_lat,
                        const double & sub_solar_lon,
                        const double & lat,
                        const double & lon) const = 0;

    /// Return cosine of phase angle
    /**
     * @param sub_observ_lat Bodycentric subobservation latitude
     * @param sub_observ_lon Subobservation longitude
     * @param sub_solar_lat  Bodycentric subsolar latitude
     * @param sub_solar_lon  Subsolar longitude
     * @param lat            Bodycentric latitude
     * @param lon            Longitude
     * @param range          Observer range to subobservation point
     *
     * @return Cosine of phase angle.
     */
    virtual double cos_phase (const double & sub_observ_lat,
                              const double & sub_observ_lon,
                              const double & sub_solar_lat,
                              const double & sub_solar_lon,
                              const double & lat,
                              const double & lon,
                              const double & range) const = 0;

    /// Radius of curvature of the meridian.
    /**
     * @param lat Bodycentric latitude
     *
     * @return Radius of curvature of the meridian.
     */
    virtual double M (const double & lat) = 0;

    /// Radius of curvature in the direction of the prime vertical
    /// perpendicular to the direction of the meridian.
    /**
     * @param lat Bodycentric latitude
     *
     * @return Radius of curvature in the direction of the prime
     *         vertical  perpendicular to the direction of the
     *         meridian.
     */
    virtual double N (const double & lat) = 0;

  private:

    /// Flag that states whether the body rotation is prograde or
    /// retrograde.
    /**
     * prograde == @c true, retrograde == @c false
     */
    bool prograde_;

  };

  // ImageFactory specialization of the ValuePtr traits template.
  template<>
  struct VP_traits<BodyData>
  {
    static BodyData * clone (const BodyData * p)
    {
      return p->clone ();
    }
  };

}

#endif  /* MARC_BODY_DATA_H */
