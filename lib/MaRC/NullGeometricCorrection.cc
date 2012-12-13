// $Id: NullGeometricCorrection.cc,v 1.2 2004/07/06 00:01:41 othman Exp $

#include <MaRC/NullGeometricCorrection.h>


void
MaRC::NullGeometricCorrection::image_to_object (double & /* line */,
                                                double & /* sample */)
{
  // No correction.
}

void
MaRC::NullGeometricCorrection::object_to_image (double & /* line */,
                                                double & /* sample */)
{
  // No correction.
}

MaRC::GeometricCorrection *
MaRC::NullGeometricCorrection::clone (void) const
{
  // Stateless so a default constructed instance will suffice.
  return new NullGeometricCorrection;
}
