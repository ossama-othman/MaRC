#include "Mu0Image.h"
#include "OblateSpheroid.h"
#include "Constants.h"

#include <cmath>


MaRC::Mu0Image::Mu0Image (const OblateSpheroid & body,
                          double sub_solar_lat,
                          double sub_solar_lon)
  : VirtualImage (10000, 0),
    body_ (body),
    sub_solar_lat_ (sub_solar_lat * C::degree), // Radians
    sub_solar_lon_ (sub_solar_lon * C::degree) // Radians
{
}

bool
MaRC::Mu0Image::read_data_i (const double & lat,
                             const double & lon,
                             double & data) const
{
  data = this->body_.mu0 (this->sub_solar_lat_,
                          this->sub_solar_lon_,
                          lat,
                          lon);

  return true;
}

bool
MaRC::Mu0Image::is_visible (const double & lat,
                            const double & lon) const
{
  // FIXME: This is ugly.  The visibility check is specific to an oblate
  // spheroid.  It should really be moved to OblateSpheroid strategy.

  const double latg = this->body_.graphic_latitude (lat);

  // When creating cosine of incidence angle map use terminator
  // instead of limb so that entire incidence angle map is created
  // as opposed to just the area visible to the observer.
  const double cosine = ::tan (latg) * ::tan (this->sub_solar_lat_);

  if (cosine >= -1 && cosine <= 1)  // Partial range of longitudes are visible
    {
      const double lower = this->sub_solar_lon_ - ::fabs (::acos (-cosine));
      const double upper = this->sub_solar_lon_ + ::fabs (::acos (-cosine));

      double l = lon;

      if (l < lower)
        l += C::_2pi;
      else if (l > upper)
        l -= C::_2pi;

      if (l >= lower && l <= upper)
        return true;
    }
  else if (cosine < -1)            // Full 360 degree visible longitude range
    return true;

  return false;
}
