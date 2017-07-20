// $Id: VirtualImage.cc,v 1.2 2004/07/06 00:01:41 othman Exp $

#include "VirtualImage.h"

MaRC::VirtualImage::VirtualImage (double s, double o)
  : scale_ (s),
    offset_ (o)
{
}

bool
MaRC::VirtualImage::read_data (const double & lat,
                               const double & lon,
                               double & data) const
{
  // FIXME: This is ugly.  The visibility check is specific to an oblate
  // spheroid.  It should really be moved to OblateSpheroid strategy.
  if (this->is_visible (lat, lon)
      && this->read_data_i (lat, lon, data))
    {
      // CHECK ME: Is this right?
      data = data * this->scale () + this->offset ();

      return true;
    }

  return false;
}

bool
MaRC::VirtualImage::is_visible (const double & /* lat */,
                                const double & /* lon */) const
{
  return true;  // Always visible by default.
}

double
MaRC::VirtualImage::scale (void) const
{
  return this->scale_;
}

double
MaRC::VirtualImage::offset (void) const
{
  return this->offset_;
}
