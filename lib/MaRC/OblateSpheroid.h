//   -*- C++ -*-

//==========================================================================
/**
 *  @file OblateSpheroid.h
 *
 *  $Id: OblateSpheroid.h,v 1.4 2004/10/14 08:54:29 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================

#ifndef MARC_OBLATE_SPHEROID_H
#define MARC_OBLATE_SPHEROID_H

#include <MaRC/Geometry.h>
#include <MaRC/BodyData.h>
#include <cmath>


namespace MaRC
{
  /**
   * @class OblateSpheroid
   *
   * @brief Class representing a body modeled as an oblate spheroid.
   *
   * An oblate spheroid is an ellipsoidal body with potentially
   * different equatorial and polar radii.
   */
  class OblateSpheroid : public BodyData
  {
  public:

    /// Constructor
    /**
     * @param prograde   Flag that states whether body rotation is
     *                   prograde or retrograde.
     * @param eq_rad     Equatorial radius in kilometers.
     * @param pol_rad    Polar radius in kilometers.
     * @param flattening Flattening ((eq_rad - pol_rad) / eq_rad).
     */
    OblateSpheroid (bool prograde,
                    double eq_rad,
                    double pol_rad,
                    double flattening);

    /// Destructor
    virtual ~OblateSpheroid (void);

    /// Clone operation that polymorphically copies the concrete
    /// @c BodyData object.
    /**
     * This clone operation is generally used in conjunction with the
     * VP_traits<BodyData> template specialization to prevent the
     * slicing that occurs when copying through a copy constructor
     * instead.
     */
    virtual BodyData * clone (void) const;

    /// Get the equatorial radius of the body.
    double eq_rad (void) const
    {
      return this->eq_rad_;
    }

    /// Get the polar radius of the body.
    double pol_rad (void) const
    {
      return this->pol_rad_;
    }

    /// Get the flattening.
    double flattening (void) const
    {
      return this->flattening_;
    }

    /// Get the first eccentricity.
    double first_eccentricity (void) const
    {
      return this->first_eccentricity_;
    }

    /**
     * @name MaRC::BodyData Virtual Methods
     *
     * Methods required by the MaRC::BodyData abstract base class.
     */
    //@{
    virtual double centric_radius (const double & lat) const;

    virtual double centric_latitude (const double & lat) const;

    virtual double graphic_latitude (const double & lat) const;

    virtual double mu (const double & sub_observ_lat,
                       const double & sub_observ_lon,
                       const double & lat,
                       const double & lon,
                       const double & range) const;

    virtual double mu0 (const double & sub_solar_lat,
                        const double & sub_solar_lon,
                        const double & lat,
                        const double & lon) const;

    virtual double cos_phase (const double & sub_observ_lat,
                              const double & sub_observ_lon,
                              const double & sub_solar_lat,
                              const double & sub_solar_lon,
                              const double & lat,
                              const double & lon,
                              const double & range) const;

    virtual double M (const double & lat);

    virtual double N (const double & lat);
    //@}

    /// Intersection of tri-axial ellipsoid with line.
    /**
     * Line = vec + k * dvec, where k will computed.
     *
     * @param vec  Vector from ellipsoid center to observer
     * @param dvec Vector along line
     * @param lat  Bodycentric (e.g. planetocentric) latitude
     * @param lon  Bodycentric (e.g. planetocentric) east longitude
     *
     * @return Zero if ellipse intersection found, non-zero otherwise.
     *
     * @note Longitude is east longitude.
     * @note Zero longitude is at x = 0, y < 0, y is increasing
     *       moving away from observer.
     */
    int ellipse_intersection (const DVector & vec,
                              const DVector & dvec,
                              double & lat,
                              double & lon) const;

  private:

    /// Initialize equatorial radius, polar radius and flattening
    /// using at least two of the given parameters.
    /**
     * @param eq_rad     Equatorial radius in kilometers.
     * @param pol_rad    Polar radius in kilometers.
     * @param flattening Flattening ((eq_rad - pol_rad) / eq_rad).
     */
    void initialize_radii (double eq_rad,
                           double pol_rad,
                           double flattening);

  private:

    /// Equatorial radius (kilometers).
    double eq_rad_;

    /// Polar radius (kilometers).
    double pol_rad_;

    /// Flattening.
    /**
     * flattening = (eq_rad - pol_rad) / eq_rad
     *
     *     flattening == 0 : Sphere
     * 0 < flattening <  1 : Oblate Spheroid
     *
     *     flattening <  0 : eq_rad < pol_rad : INVALID
     * 1 < flattening      : pol_rad < 0      : INVALID
     *     flattening == 1 : Disc             : INVALID
     */
    double flattening_;

    /// First eccentricity.
    /**
     * (1 - (1 - flattening) ^ 2)
     */
    double first_eccentricity_;

  };


  /************************************
   Inline functions (very heavily used)
   ************************************/

  // These can be inlined into PhotoImage
  // because PhotoImage contains an OblateSpheroid directly
  // not a reference or pointer

  inline double
  MaRC::OblateSpheroid::centric_radius (const double & lat) const
  {
    const double er2     = this->eq_rad_ * this->eq_rad_;
    const double pr2     = this->pol_rad_ * this->pol_rad_;
    const double sin_lat = ::sin (lat);
  
    return
      this->eq_rad_ * this->pol_rad_ /
      ::sqrt ((er2 - pr2) * sin_lat * sin_lat + pr2);
  }
  
  inline double
  MaRC::OblateSpheroid::centric_latitude (const double & lat) const
  {
    return
      ::atan (this->pol_rad_ * this->pol_rad_ / (this->eq_rad_ * this->eq_rad_)
              * ::tan (lat));
  }
  
  inline double
  MaRC::OblateSpheroid::graphic_latitude (const double & lat) const
  {
    return
      ::atan (this->eq_rad_ * this->eq_rad_ / (this->pol_rad_ * this->pol_rad_)
              * ::tan (lat));
  }
  
  inline double
  MaRC::OblateSpheroid::mu (const double & sub_observ_lat,
                            const double & sub_observ_lon,
                            const double & lat,
                            const double & lon,
                            const double & range) const
  {
    // Compute the local normal-observer angle - Emission Angle (Mu)
  
    const double latg = this->graphic_latitude (lat);
    const double ellipse_radius = this->centric_radius (lat);
  
    return ((range * ::sin (sub_observ_lat) * ::sin (latg) - ellipse_radius *
  	   ::cos (lat - latg)) + range * ::cos (sub_observ_lat) *
  	  ::cos (latg) * ::cos (sub_observ_lon - lon)) /
      // dot product (above)
      // divided by the magnitude of vector
      // from observer to point on body
      ::sqrt (range * range + ellipse_radius * ellipse_radius -
              2 * range * ellipse_radius *
              (::sin (sub_observ_lat) * ::sin (lat) +
               ::cos (sub_observ_lat) *
               ::cos (lat) * ::cos (sub_observ_lon - lon)));
  }
  
  inline double
  MaRC::OblateSpheroid::mu0 (const double & sub_solar_lat,
                             const double & sub_solar_lon,
                             const double & lat,
                             const double & lon) const
  {
    // Compute the sun-local normal angle - Incidence Angle (Mu0)
  
    const double latg = this->graphic_latitude (lat);
  
    return ::sin (sub_solar_lat) * ::sin (latg) +
      ::cos (sub_solar_lat) * ::cos (latg) * ::cos (sub_solar_lon - lon);
    // The above equation assumes the sun to be an infinite distance away.
  }

} /* namespace MaRC */
#endif  /* MARC_OBLATE_SPHEROID_H */
