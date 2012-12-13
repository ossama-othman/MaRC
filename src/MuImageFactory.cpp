// $Id: MuImageFactory.cpp,v 1.1 2004/07/03 10:59:42 othman Exp $

#include "MuImageFactory.h"

#include "MaRC/MuImage.h"

MaRC::MuImageFactory::MuImageFactory (const OblateSpheroid & body,
                                      double sub_observ_lat,
                                      double sub_observ_lon,
                                      double range)
  : body_ (body),
    sub_observ_lat_ (sub_observ_lat),
    sub_observ_lon_ (sub_observ_lon),
    range_ (range)
{
}

MaRC::SourceImage *
MaRC::MuImageFactory::make (void)
{
  return new MaRC::MuImage (this->body_,
                            this->sub_observ_lat_,
                            this->sub_observ_lon_,
                            this->range_);
}

MaRC::ImageFactory *
MaRC::MuImageFactory::clone (void) const
{
  // Simple copy construction will suffice.
  return new MuImageFactory (*this);
}
