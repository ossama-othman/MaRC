// $Id: MuImage.cc,v 1.3 2004/07/06 00:01:41 othman Exp $

#include "MuImage.h"
#include "OblateSpheroid.h"
#include "Constants.h"

#include <cmath>


MaRC::MuImage::MuImage (const OblateSpheroid & body,
                        double sub_observ_lat,
                        double sub_observ_lon,
                        double range)
  : VirtualImage (10000, 0),
    body_ (body),
    sub_observ_lat_ (sub_observ_lat * C::degree), // Radians
    sub_observ_lon_ (sub_observ_lon * C::degree), // Radians
    range_ (range)
{
}

bool
MaRC::MuImage::read_data_i (const double & lat,
                            const double & lon,
                            double & data) const
{
  data = this->body_.mu (this->sub_observ_lat_,
                         this->sub_observ_lon_,
                         lat,
                         lon,
                         this->range_);

  return true;
}

bool
MaRC::MuImage::is_visible (const double & lat,
                           const double & lon) const
{
  return MaRC::MuImage::is_visible_i (this->body_,
                                      this->sub_observ_lat_,
                                      this->sub_observ_lon_,
                                      lat,
                                      lon,
                                      this->range_);
}

bool
MaRC::MuImage::is_visible_i (const OblateSpheroid & body,
                             const double & sub_observ_lat,
                             const double & sub_observ_lon,
                             const double & lat,
                             const double & lon,
                             const double & range)
{
  const double latg = body.graphic_latitude (lat);

  const double radius = body.centric_radius (lat);

  const double cosine =
    (radius * ::cos (lat - latg) -
     range * ::sin (sub_observ_lat) *
     ::sin (latg)) /
    range /::cos (sub_observ_lat) / ::cos (latg);

  if (cosine >= -1 && cosine <= 1)  // Partial range of longitudes are visible
    {
      const double lower = sub_observ_lon - ::fabs (::acos (cosine));
      const double upper = sub_observ_lon + ::fabs (::acos (cosine));

      // Now check if longitude at given latitude is visible
      double l = lon;

      if (l < lower)
        l += C::_2pi;
      else if (l > upper)
        l -= C::_2pi;

      if (l >= lower && l <= upper)
        return true;
    }
  else if (cosine < -1)             // Full 360 degree visible longitude range
    return true;

  return false;
}
