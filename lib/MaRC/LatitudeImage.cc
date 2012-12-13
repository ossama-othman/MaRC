// $Id: LatitudeImage.cc,v 1.2 2004/07/03 10:59:42 othman Exp $

#include "LatitudeImage.h"
#include "BodyData.h"
#include "Constants.h"


MaRC::LatitudeImage::LatitudeImage (const MaRC::ValuePtr<BodyData> & body,
                                    bool graphic_latitudes)
  : body_ (body),
    graphic_latitudes_ (graphic_latitudes)
{
}

bool
MaRC::LatitudeImage::read_data_i (const double & lat,
                                  const double & /* lon */,
                                  double & data) const
{
  if (this->graphic_latitudes_)
    data = this->body_->graphic_latitude (lat);
  else
    data = lat;

  data /=  C::degree;  // Convert radians to degrees.

  return true;
}
