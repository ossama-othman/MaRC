#include "VirtualImage.h"

MaRC::VirtualImage::VirtualImage(double s, double o)
  : scale_(s)
  , offset_(o)
{
}

bool
MaRC::VirtualImage::read_data(double lat, double lon, double & data) const
{
  /**
   * @todo This is ugly.  The visibility check is specific to an
   *       oblate spheroid.  It should really be moved to
   *       the @c OblateSpheroid strategy.
   */
  if (this->is_visible (lat, lon)
      && this->read_data_i (lat, lon, data)) {
    /**
     * @todo Is this right?
     */
    data = data * this->scale_ + this->offset_;

    return true;
  }

  return false;
}

bool
MaRC::VirtualImage::is_visible(double /* lat */, double /* lon */) const
{
  return true;  // Always visible by default.
}
