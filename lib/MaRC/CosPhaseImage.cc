// $Id: CosPhaseImage.cc,v 1.3 2004/07/06 00:01:41 othman Exp $

#include "CosPhaseImage.h"
#include "MuImage.h"
#include "OblateSpheroid.h"
#include "Constants.h"

MaRC::CosPhaseImage::CosPhaseImage (const OblateSpheroid & body,
                                    double sub_observ_lat,
                                    double sub_observ_lon,
                                    double sub_solar_lat,
                                    double sub_solar_lon,
                                    double range)
  : VirtualImage (10000, 0),
    body_ (body),
    sub_observ_lat_ (sub_observ_lat * C::degree), // Radians
    sub_observ_lon_ (sub_observ_lon * C::degree), // Radians
    sub_solar_lat_ (sub_solar_lat * C::degree), // Radians
    sub_solar_lon_ (sub_solar_lon * C::degree), // Radians
    range_ (range)
{
}

bool
MaRC::CosPhaseImage::read_data_i (const double & lat,
                                  const double & lon,
                                  double & data) const
{
  data = this->body_.cos_phase (this->sub_observ_lat_,
                                this->sub_observ_lon_,
                                this->sub_solar_lat_,
                                this->sub_solar_lon_,
                                lat,
                                lon,
                                this->range_);

  return true;
}

bool
MaRC::CosPhaseImage::is_visible (const double & lat,
                                 const double & lon) const
{
  // This implementation is as the same as the one used by the MuImage
  // class.

  return MaRC::MuImage::is_visible_i (this->body_,
                                      this->sub_observ_lat_,
                                      this->sub_observ_lon_,
                                      lat,
                                      lon,
                                      this->range_);
}
