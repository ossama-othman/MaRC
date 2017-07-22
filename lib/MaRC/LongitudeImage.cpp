#include "LongitudeImage.h"
#include "Constants.h"

bool
MaRC::LongitudeImage::read_data_i (double /* lat */,
                                   double lon,
                                   double & data) const
{
  data = lon / C::degree;  // Convert radians to degrees.

  if (data < 0)
    data += 360;
  else if (data >= 360)
    data -= 360;

  return true;
}
