// $Id: ImageFactory.cpp,v 1.1 2004/07/03 10:59:42 othman Exp $

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
