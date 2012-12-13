// $Id: Mu0ImageFactory.cpp,v 1.1 2004/07/03 10:59:42 othman Exp $

#include "Mu0ImageFactory.h"

#include "MaRC/Mu0Image.h"

MaRC::Mu0ImageFactory::Mu0ImageFactory (const OblateSpheroid & body,
                                        double sub_solar_lat,
                                        double sub_solar_lon)
  : body_ (body),
    sub_solar_lat_ (sub_solar_lat),
    sub_solar_lon_ (sub_solar_lon)
{
}

MaRC::SourceImage *
MaRC::Mu0ImageFactory::make (void)
{
  return new MaRC::Mu0Image (this->body_,
                             this->sub_solar_lat_,
                             this->sub_solar_lon_);
}

MaRC::ImageFactory *
MaRC::Mu0ImageFactory::clone (void) const
{
  // Simple copy construction will suffice.
  return new Mu0ImageFactory (*this);
}
