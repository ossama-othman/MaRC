#include "LongitudeImageFactory.h"

#include "MaRC/LongitudeImage.h"


MaRC::SourceImage *
MaRC::LongitudeImageFactory::make (void)
{
  return new MaRC::LongitudeImage;
}

MaRC::ImageFactory *
MaRC::LongitudeImageFactory::clone (void) const
{
  // We're stateless.  Just create a default constructed instance.
  return new LongitudeImageFactory;
}
