#include "ImageFactory.h"

#include <limits>


MaRC::ImageFactory::~ImageFactory (void)
{
}

MaRC::ImageFactory::ImageFactory (void)
  : minimum_ (-std::numeric_limits<double>::max ()),
    maximum_ ( std::numeric_limits<double>::max ())
{
}
