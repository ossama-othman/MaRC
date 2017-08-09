// $Id: SimpleCylindrical.cc,v 1.7 2004/10/14 08:54:29 othman Exp $

#include "MaRC/SimpleCylindrical.h"
#include "MaRC/Constants.h"
#include "MaRC/BodyData.h"

#include <limits>
#include <cmath>

template <typename T>
MaRC::SimpleCylindrical<T>::SimpleCylindrical (const ValuePtr<BodyData> & body,
                                               double lo_lat,
                                               double hi_lat,
                                               double lo_lon,
                                               double hi_lon,
                                               bool graphic_lat)
  : MapFactory<T> (),
    body_ (body),
    lo_lat_ (C::pi_2),
    hi_lat_ (-C::pi_2),
    lo_lon_ (0),
    hi_lon_ (C::_2pi),
    graphic_lat_ (graphic_lat)
{
  // Set latitude range.
  // All latitudes are fed to SimpleCylindrical as CENTRIC.
  if (lo_lat >= -90 && lo_lat <= 90)
    this->lo_lat_ = lo_lat * C::degree;

  if (hi_lat >= -90 && hi_lat <= 90)
    this->hi_lat_ = hi_lat * C::degree;

  // Convert to GRAPHIC latitude if requested.
  if (graphic_lat)
    {
      this->lo_lat_ = this->body_->graphic_latitude (this->lo_lat_);
      this->hi_lat_ = this->body_->graphic_latitude (this->hi_lat_);
    }

  // Set longitude range
  if (lo_lon >= -360 && lo_lon <= 360)
    this->lo_lon_ = lo_lon * C::degree;

  if (hi_lon >= -360 && hi_lon <= 360)
    this->hi_lon_ = hi_lon * C::degree;

  if (this->lo_lon_ > this->hi_lon_)
    this->lo_lon_ -= C::_2pi;
}

template <typename T>
MaRC::SimpleCylindrical<T>::~SimpleCylindrical (void)
{
}

template <typename T>
MaRC::MapFactory<T> *
MaRC::SimpleCylindrical<T>::clone (void) const
{
  // Simple copy construction will suffice.
  return new MaRC::SimpleCylindrical<T> (*this);
}

template <typename T>
const char *
MaRC::SimpleCylindrical<T>::projection_name (void) const
{
  static const char name[] = "Simple Cylindrical";

  return name;
}

template <typename T>
typename MaRC::SimpleCylindrical<T>::map_type *
MaRC::SimpleCylindrical<T>::make_map (SourceImage & source,
                                      unsigned int samples,
                                      unsigned int lines,
                                      double minimum,
                                      double maximum)
{
  std::auto_ptr<map_type> map (new map_type (samples, lines));

  // Conversion factor -- latitudes per line
  const double cf = (this->hi_lat_ - this->lo_lat_) / lines;

  const unsigned int nelem = samples * lines;

  unsigned int offset = 0;

  for (unsigned int k = 0; k < lines; ++k)
    {
      // Compute latitude at center of pixel.
      double lat = (k + 0.5) * cf + this->lo_lat_;

      // Convert to CENTRIC Latitude
      if (this->graphic_lat_)
        lat = this->body_->centric_latitude (lat);

      for (unsigned int i = 0; i < samples; ++i, ++offset)
        {
          const double lon = this->get_longitude (i, samples);

          const unsigned char percent_complete =
            static_cast<unsigned char> ((offset + 1) * 100 / nelem);

          double data = 0;
          if (this->plot (source,
                          lat,
                          lon,
                          minimum,
                          maximum,
                          percent_complete,
                          data))
            (*map)[offset] = static_cast<T> (data);
        }
    }

  return map.release ();
}

template <typename T>
typename MaRC::SimpleCylindrical<T>::grid_type *
MaRC::SimpleCylindrical<T>::make_grid (unsigned int samples,
                                       unsigned int lines,
                                       float lat_interval,
                                       float lon_interval)
{
  std::auto_ptr<grid_type> grid (new grid_type (samples, lines));

  const double lo_lat = this->lo_lat_ /= C::degree;  // Convert back to degrees
  const double hi_lat = this->hi_lat_ /= C::degree;
  const double lo_lon = this->lo_lon_ /= C::degree;
  const double hi_lon = this->hi_lon_ /= C::degree;

  // Line-to-latitude ratio.
  const double lr = lines / (hi_lat - lo_lat);

  static const typename grid_type::data_type white =
    std::numeric_limits<typename grid_type::data_type>::max ();

 // Draw latitude lines
  for (float n = -90 + lat_interval; n < 90; n += lat_interval)
    {
      const double k = ::rint ((n - lo_lat) * lr);

      if (k >= 0 && k < static_cast<double> (lines))
        {
          const unsigned int offset =
            static_cast<unsigned int> (k) * samples;

          for (unsigned int i = 0; i < samples; ++i)
            (*grid)[offset + i] = white;
        }
    }

  // Sample-to-longitude ratio.
  const double sr = samples / (hi_lon - lo_lon);

  // Draw longitude lines.
  for (float m = 360; m > 0; m -= lon_interval)
    {
      // lo_lon_2 is a work-around for lo_lon_ > hi_lon_ problems
      double lo_lon_2;

      if (m - lo_lon > 360)
        lo_lon_2 = lo_lon + 360;
      else
        lo_lon_2 = lo_lon;

      int i;

      if (this->body_->prograde ())
        i = samples - static_cast<int> (::rint ((m - lo_lon_2) * sr));
      else
        i = static_cast<int> (::rint ((m - lo_lon_2) * sr));

      if (i >= 0 && static_cast<unsigned int> (i) < samples)
        {
          for (unsigned int k = 0; k < lines; ++k)
            (*grid) (static_cast<unsigned int> (i), k) = white;
        }
    }

  return grid.release ();
}

template <typename T>
inline double
MaRC::SimpleCylindrical<T>::get_longitude (unsigned int i,
                                           unsigned int samples) const
{
  double lon;

  // PROGRADE ----> longitudes increase to the left
  // RETROGRADE --> longitudes increase to the right

  // Compute longitude at center of pixel.

  if (this->body_->prograde ())
    lon = (i + 0.5) / samples * (lo_lon_ - hi_lon_) + hi_lon_;
  else
    lon = (i + 0.5) / samples * (hi_lon_ - lo_lon_) + lo_lon_;

  return lon;
}
