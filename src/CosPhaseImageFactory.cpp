// $Id: CosPhaseImageFactory.cpp,v 1.1 2004/07/03 10:59:42 othman Exp $

#include "CosPhaseImageFactory.h"

#include "MaRC/CosPhaseImage.h"

MaRC::CosPhaseImageFactory::CosPhaseImageFactory (const OblateSpheroid & body,
                                                  double sub_observ_lat,
                                                  double sub_observ_lon,
                                                  double sub_solar_lat,
                                                  double sub_solar_lon,
                                                  double range)
  : body_ (body),
    sub_observ_lat_ (sub_observ_lat),
    sub_observ_lon_ (sub_observ_lon),
    sub_solar_lat_ (sub_solar_lat),
    sub_solar_lon_ (sub_solar_lon),
    range_ (range)
{
}

MaRC::SourceImage *
MaRC::CosPhaseImageFactory::make (void)
{
  return new MaRC::CosPhaseImage (this->body_,
                                  this->sub_observ_lat_,
                                  this->sub_observ_lon_,
                                  this->sub_solar_lat_,
                                  this->sub_solar_lon_,
                                  this->range_);
}

MaRC::ImageFactory *
MaRC::CosPhaseImageFactory::clone (void) const
{
  // Simple copy construction will suffice.
  return new CosPhaseImageFactory (*this);
}
