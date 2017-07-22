//   -*- C++ -*-

//==========================================================================
/**
 *  @file OblateSpheroid.h
 *
 *  @author Ossama Othman
 */
//==========================================================================

#ifndef MARC_OBLATE_SPHEROID_H
#define MARC_OBLATE_SPHEROID_H

#include <MaRC/BodyData.h>
#include <MaRC/Vector.h>

namespace MaRC
{
  typedef Vector<double, 3> DVector;

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
    virtual double centric_radius (double lat) const;

    virtual double centric_latitude (double lat) const;

    virtual double graphic_latitude (double lat) const;

    virtual double mu (double sub_observ_lat,
                       double sub_observ_lon,
                       double lat,
                       double lon,
                       double range) const;

    virtual double mu0 (double sub_solar_lat,
                        double sub_solar_lon,
                        double lat,
                        double lon) const;

    virtual double cos_phase (double sub_observ_lat,
                              double sub_observ_lon,
                              double sub_solar_lat,
                              double sub_solar_lon,
                              double lat,
                              double lon,
                              double range) const;

    virtual double M (double lat);

    virtual double N (double lat);
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

}

#endif  /* MARC_OBLATE_SPHEROID_H */
