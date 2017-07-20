#include "LatitudeImageFactory.h"

#include "MaRC/LatitudeImage.h"

MaRC::LatitudeImageFactory::LatitudeImageFactory (
  const MaRC::ValuePtr<BodyData> & body,
  bool graphic_latitudes)
  : body_ (body),
    graphic_latitudes_ (graphic_latitudes)
{
}

MaRC::SourceImage *
MaRC::LatitudeImageFactory::make (void)
{
  return new MaRC::LatitudeImage (this->body_,
                                  this->graphic_latitudes_);
}

MaRC::ImageFactory *
MaRC::LatitudeImageFactory::clone (void) const
{
  // Simple copy construction will suffice.
  return new LatitudeImageFactory (*this);
}
