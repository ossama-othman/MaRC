// $Id: LongitudeImage.cc,v 1.3 2005/11/17 00:12:29 othman Exp $

#include "LongitudeImage.h"
#include "Constants.h"

#include <cmath>

bool
MaRC::LongitudeImage::read_data_i (const double & /* lat */,
                                   const double & lon,
                                   double & data) const
{
  // Force longitudes to be in 360 degree range.
  data = ::fmod (lon, C::_2pi) / C::degree;

  return true;
}
