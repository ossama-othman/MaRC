// $Id: Orthographic.cc,v 1.8 2004/10/14 08:54:29 othman Exp $

#include "MaRC/Orthographic.h"
#include "MaRC/OblateSpheroid.h"
#include "MaRC/Constants.h"
#include "MaRC/Geometry.h"
#include "MaRC/NaN.h"
//#include "misc.h"

#include <sstream>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <memory>


template <typename T>
MaRC::Orthographic<T>::Orthographic (
  const ValuePtr<OblateSpheroid> & body,
  double sub_observ_lat,
  double sub_observ_lon,
  double PA,
  double km_per_pixel,
  const OrthographicCenter & center)
  : MapFactory<T> (),
    body_ (body),
    sub_observ_lat_ (0),
    sub_observ_lon_ (0),
    PA_ (0),
    km_per_pixel_ (-1),
    sample_center_ (MaRC::NaN),
    line_center_ (MaRC::NaN),
    lat_at_center_ (MaRC::NaN),
    lon_at_center_ (MaRC::NaN),
    polar_ (false)
{
  if (sub_observ_lat >= -90 && sub_observ_lat <= 90)
    this->sub_observ_lat_ = sub_observ_lat;

  if (sub_observ_lon >= -360 && sub_observ_lon <= 360)
    {
      this->sub_observ_lon_ = sub_observ_lon;

      if (this->sub_observ_lon_ < 0)
        this->sub_observ_lon_ += 360;
    }

  if (PA >= -360 && PA <= 360)
    this->PA_ = PA;

  if (::fabs (::fabs (this->sub_observ_lat_) - 90) < 1e-5)
    {
      std::cout << "Assuming POLAR ORTHOGRAPHIC projection." << std::endl;

      if ((this->sub_observ_lat_ > 0 && this->body_->prograde ())
          || (this->sub_observ_lat_ < 0 && !this->body_->prograde ()))
        {
          if (this->body_->prograde ())
            this->PA_ = 180;
          else
            this->PA_ = 0;
        }
      else
        {
          if (this->body_->prograde ())
            this->PA_ = 0;
          else
            this->PA_ = 180;
        }

      if (this->sub_observ_lat_ > 0)
        this->sub_observ_lat_ = 90;
      else
        this->sub_observ_lat_ = -90;

      this->sub_observ_lon_ = 0;
      this->polar_ = true;
    }

  this->sub_observ_lat_  *= C::degree; // Convert to radians
  this->sub_observ_lon_  *= C::degree;
  this->PA_              *= C::degree;

  if (km_per_pixel > 0)
    this->km_per_pixel_ = km_per_pixel;

  if (center.geometry == CENTER_GIVEN)
    {
      this->sample_center_ = center.sample_lat_center;
      this->line_center_   = center.line_lon_center;
    }

  // Latitude and Longitude at center of map given (in addition to
  // KM/pixel)
  else if (center.geometry == LAT_LON_GIVEN)
    {
      if (center.sample_lat_center >= -90
          && center.sample_lat_center <= 90)
        this->lat_at_center_ = center.sample_lat_center * C::degree; // Radians
      else
        this->lat_at_center_ = this->sub_observ_lat_;

      if (center.line_lon_center >= -360
          && center.line_lon_center <= 360)
        {
          this->lon_at_center_ = center.line_lon_center; // Degrees

          if (this->lon_at_center_ < 0)
            this->lon_at_center_ += 360;

          this->lon_at_center_ *= C::degree; // Convert to radians
        }
      else
        this->lon_at_center_ = this->sub_observ_lon_;
    }
}

template <typename T>
const char *
MaRC::Orthographic<T>::projection_name (void) const
{
  static const char name[] = "Orthographic";

  return name;
}

template <typename T>
MaRC::MapFactory<T> *
MaRC::Orthographic<T>::clone (void) const
{
  // Simple copy construction will suffice.
  return new MaRC::Orthographic<T> (*this);
}

template <typename T>
typename MaRC::Orthographic<T>::map_type *
MaRC::Orthographic<T>::make_map (const SourceImage & source,
                                 unsigned int samples,
                                 unsigned int lines,
                                 double minimum,
                                 double maximum)
{
  this->init (samples, lines);

  std::unique_ptr<map_type> map (new map_type (samples, lines));

  const unsigned int nelem = samples * lines;

  DVector ImgCoord (3), Rotated (3);

  const DMatrix rotY (MaRC::Geometry::RotYMatrix (-this->PA_));
  const DMatrix rotX (MaRC::Geometry::RotXMatrix (this->sub_observ_lat_));

  const double a2   = this->body_->eq_rad () * this->body_->eq_rad ();
  const double c2   = this->body_->pol_rad () * this->body_->pol_rad ();
  const double diff = a2 - c2;

  // "a" coefficient of the Quadratic Formula.
  const double CA = diff * ::pow (::sin (this->sub_observ_lat_), 2.0) + c2;

  unsigned int offset = 0;

  for (unsigned int k = 0; k < lines; ++k)
    {
      const double z = (k + 0.5 - this->line_center_) * this->km_per_pixel_;

      for (unsigned int i = 0; i < samples; ++i, ++offset)
        {
          double x = (i + 0.5 - this->sample_center_) * this->km_per_pixel_;
          double zz;

          if (!this->polar_)
            {
              ImgCoord[0] = x;
              ImgCoord[1] = 0;
              ImgCoord[2] = z;
              Rotated = rotY * ImgCoord;
              x  = Rotated[0];
              zz = Rotated[2];
            }
          else
            zz = z;

          const double CB = diff * zz * ::sin (2 * this->sub_observ_lat_);
          const double CC =
            a2 * zz * zz + c2 * x * x - a2 * c2 - diff *
            zz * zz * ::pow (::sin (this->sub_observ_lat_), 2.0);

          const double discriminant = CB * CB - 4 * CA * CC;

          if (discriminant >= 0)
            {
              const double y1 = (-CB + ::sqrt (discriminant)) / 2 / CA;
              const double y2 = (-CB - ::sqrt (discriminant)) / 2 / CA;
              double y = std::min (y1, y2);
              ImgCoord[0] = x;
              ImgCoord[1] = y;
              ImgCoord[2] = zz;
              Rotated = rotX * ImgCoord;
              if (this->polar_)
                {
                  // Rotate about z-axis by (-this->PA_).
                  x =  Rotated[0] * ::cos (-this->PA_) +
                    Rotated[1] * ::sin (-this->PA_);

                  y = -Rotated[0] * ::sin (-this->PA_) +
                    Rotated[1] * ::cos (-this->PA_);

                  zz=  Rotated[2];
                }
              else
                {
                  x = Rotated[0];
                  y = Rotated[1];
                  zz= Rotated[2];
                }

              const double lat = ::atan2 (zz, ::sqrt (x * x + y * y));

              double lon;

              if (this->body_->prograde ())
                lon = this->sub_observ_lon_ - ::atan2 (-x, y) + C::pi;
              else
                lon = this->sub_observ_lon_ + ::atan2 (-x, y) - C::pi;

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
    }

  std::cout
    << "Body center in ORTHOGRAPHIC projection (line, sample): ("
    << this->line_center_ << ", " << this->sample_center_ << ")"
    << std::endl;

  return map.release ();
}

template <typename T>
typename MaRC::Orthographic<T>::grid_type *
MaRC::Orthographic<T>::make_grid (unsigned int samples,
                                  unsigned int lines,
                                  float lat_interval,
                                  float lon_interval)
{
  std::unique_ptr<grid_type> grid (new grid_type (samples, lines));

  int i, k, imax = 2000;
  double low_bound, high_bound, x, z;
  DVector Coord(3), T_Coord(3);
  float m, mm, mm2, n, nn;

  // Tranformation matrix to rotate about x than new y
  const DMatrix body2obs (Geometry::RotYMatrix (-this->PA_) *
                          Geometry::RotXMatrix (this->sub_observ_lat_));

  static const typename grid_type::data_type white =
    std::numeric_limits<typename grid_type::data_type>::max ();

  // Draw latitude lines
  for (n = -90; n <= 90; n += lat_interval)
    {
      nn =  n * C::degree; // Convert to radians

      if (n == 0 && ::fabs (::fabs (this->sub_observ_lat_) - C::pi_2) < 1e-6)
        {
          // Polar case
          low_bound = -C::pi;
          high_bound= C::pi;
        }
      else
        {
          // General case

          // tan (graphic lat) * tan (sub observ lat)
          const double cosine =
            this->body_->eq_rad () * this->body_->eq_rad () /
            this->body_->pol_rad () / this->body_->pol_rad () *
            ::tan (nn) * ::tan (this->sub_observ_lat_);

          if (cosine >= -1 && cosine <= 1)
            {
              low_bound  = this->sub_observ_lon_ - ::fabs (::acos (-cosine));
              high_bound = this->sub_observ_lon_ + ::fabs (::acos (-cosine));
            }
          else if (cosine > 1)
            {
              low_bound  = -C::pi;
              high_bound =  C::pi;
            }
          else
            continue;
        }

      const double radius = this->body_->centric_radius (nn);

      for (m = 0; m < imax; ++m)
        {
          mm = static_cast<float> (m) / imax * C::degree * 360;

          if (mm < low_bound)
            mm += C::_2pi;
          else if (mm > high_bound)
            mm -= C::_2pi;

          if (mm >= low_bound && mm <= high_bound)
            {
              if (this->body_->prograde ())
                mm = this->sub_observ_lon_ + C::pi - mm;
              else
                mm -= C::pi - this->sub_observ_lon_;

              Coord[0] = radius * ::cos (nn) * ::sin (mm);
              Coord[1] =-radius * ::cos (nn) * ::cos (mm);
              Coord[2] = radius * ::sin (nn);

              T_Coord = body2obs * Coord;

              x = T_Coord[0] / this->km_per_pixel_;
              z = T_Coord[2] / this->km_per_pixel_;

              i = static_cast<int> (::rint (this->sample_center_ - x));
              k = static_cast<int> (::rint (this->line_center_ + z));

              if (i >= 0 && static_cast <unsigned int> (i) < samples
                  && k >= 0 && static_cast <unsigned int> (k) < lines)
                {
                  const unsigned int index =
                    static_cast<unsigned int> (k) * samples +
                    static_cast<unsigned int> (i);

                  (*grid)[index] = white;
                }
            }
        }
    } // End draw latitude lines

 // Draw longitude lines
  imax = 1000;
  for (m = 0 + lon_interval; m <= 360; m += lon_interval)
    {
      mm = m * C::degree;
      for (n = 0; n < imax; ++n)
        {
          mm2 = mm;
          nn = (static_cast<float> (n) / imax * 180 - 90) * C::degree;

          // tan (graphic lat) * tan (sub observ lat)
          const double cosine =
            this->body_->eq_rad () * this->body_->eq_rad () /
            this->body_->pol_rad () / this->body_->pol_rad () * ::tan (nn) *
            ::tan (this->sub_observ_lat_);

          if (cosine >= -1 && cosine <= 1)
            {
              low_bound  = this->sub_observ_lon_ - ::fabs (::acos (-cosine));
              high_bound = this->sub_observ_lon_ + ::fabs (::acos (-cosine));
            }
          else if (cosine > 1)
            {
              low_bound  = -C::pi;
              high_bound =  C::pi;
            }
          else
            continue;

          if (mm2 < low_bound)
            mm2 += C::_2pi;
          else if (mm2 > high_bound)
            mm2 -= C::_2pi;

          if (mm2 >= low_bound && mm2 <= high_bound)
            {
              if (this->body_->prograde ())
                mm2 = this->sub_observ_lon_ + C::pi - mm2;
              else
                mm2 -= C::pi + this->sub_observ_lon_;

              const double radius = this->body_->centric_radius (nn);

              Coord[0] = radius * ::cos (nn) * ::sin (mm2);
              Coord[1] =-radius * ::cos (nn) * ::cos (mm2);
              Coord[2] = radius * ::sin (nn);

              T_Coord = body2obs * Coord;

              x = T_Coord[0] / this->km_per_pixel_;
              z = T_Coord[2] / this->km_per_pixel_;

              i = static_cast<int> (::rint (this->sample_center_ - x));
              k = static_cast<int> (::rint (this->line_center_ + z));

              if (i >= 0 && static_cast<unsigned int> (i) < samples
                  && k >= 0 && static_cast<unsigned int> (k) < lines)
                {
                  const unsigned int index =
                    static_cast<unsigned int> (k) * samples +
                    static_cast<unsigned int> (i);

                  (*grid)[index] = white;
                }
            }
        }
    } // End draw longitude lines

  return grid.release ();
}

template <typename T>
void
MaRC::Orthographic<T>::init (unsigned int samples, unsigned int lines)
{
  if (this->km_per_pixel_ <= 0)
    {
      static const double MAP_FRACTION = 0.9;

      // The largest axis of the oblate spheroid will take up at most
      // MAP_FRACTION of the smallest dimension of the map.
      this->km_per_pixel_ =
        2 * std::max (this->body_->eq_rad (), this->body_->pol_rad ()) /
        (MAP_FRACTION * std::min (samples, lines));
    }


  if (!std::isnan (this->lat_at_center_) && !std::isnan (this->lon_at_center_))
    {
      // Check if longitude at center (if supplied) is visible.

      //cosine =
      //  ::tan (this->body_->graphic_latitude (Lat)) *
      //  ::tan (this->sub_observ_lat_);

      const double cosine =
        this->body_->eq_rad () * this->body_->eq_rad () /
        this->body_->pol_rad () / this->body_->pol_rad () *
        ::tan (this->lat_at_center_) * ::tan (this->sub_observ_lat_);

      if (cosine < -1)
        {
          std::ostringstream s;
          s << "Desired LATITUDE (" << this->lat_at_center_ / C::degree
            << ") at center of image is not visible.";

          throw std::out_of_range (s.str ());
        }

      double lower = this->sub_observ_lon_ - C::pi;
      double upper = this->sub_observ_lon_ + C::pi;  // 360

      if (!this->polar_ && ::fabs (cosine) <= 1)
        {
          // Lower boundary
          lower = this->sub_observ_lon_ - ::fabs (::acos (-cosine));

          // Upper boundary
          upper = this->sub_observ_lon_ + ::fabs (::acos (-cosine));
        }

      if (this->lon_at_center_ < lower)
        this->lon_at_center_ += C::_2pi;
      else if (this->lon_at_center_ > upper)
        this->lon_at_center_ -= C::_2pi;

      if (this->lon_at_center_ < lower || this->lon_at_center_ > upper)
        {
          std::ostringstream s;
          s << "Desired LONGITUDE (" << this->lon_at_center_ / C::degree
            << ") at center of image is not visible.";

          throw std::out_of_range (s.str ());
        }

      const double shift = this->sub_observ_lon_ - this->lon_at_center_;

      double pos[3];

      if (this->body_->prograde ())
        pos[0] =
          this->body_->centric_radius (this->lat_at_center_) *
          ::cos (this->lat_at_center_) * ::sin (shift); // X
      else
        pos[0] =
          -this->body_->centric_radius (this->lat_at_center_) *
          ::cos (this->lat_at_center_) * ::sin (shift); // X

      pos[1] =
        -this->body_->centric_radius (this->lat_at_center_)
        * ::cos (this->lat_at_center_) * ::cos (shift); // Y

      pos[2] = this->body_->centric_radius (this->lat_at_center_)
        * ::sin (this->lat_at_center_);                 // Z

      this->sample_center_ =
        pos[0] * ::cos (this->PA_) + pos[1] * ::sin (this->PA_) *
        ::sin (-this->sub_observ_lat_) - pos[2] * ::sin (this->PA_) *
        ::cos (-this->sub_observ_lat_);

      //   YCenter = pos[1] * ::cos (-this->sub_observ_lat_) + pos[2] *
      //      ::sin (-this->sub_observ_lat_);
      // Drop the Y Center (not needed).

      this->line_center_ =
        pos[0] * ::sin (this->PA_) -
        pos[1] * ::sin (-this->sub_observ_lat_) * ::cos (this->PA_) +
        pos[2] * ::cos (-this->sub_observ_lat_) * ::cos (this->PA_);

      this->sample_center_ /= this->km_per_pixel_; // Convert to pixels
      this->line_center_   /= this->km_per_pixel_;

      // Shift to center of image.

      // X measured from left edge of image.
      this->sample_center_ = samples / 2.0 - this->sample_center_;

      // Z measured from bottom edge of image.
      this->line_center_   = lines   / 2.0 - this->line_center_;

      // No longer need lat/lon_at_center_, so prevent this code from
      // running again by resetting them to NaN.
      this->lat_at_center_ = MaRC::NaN;
      this->lon_at_center_ = MaRC::NaN;
    }
  else if (std::isnan (this->sample_center_) || std::isnan (this->line_center_))
    {
      this->sample_center_ = samples / 2.0;
      this->line_center_   = lines   / 2.0;
    }
}

// ------------------------------------

MaRC::OrthographicCenter::OrthographicCenter (void)
  : geometry (DEFAULT),
    sample_lat_center (MaRC::NaN),
    line_lon_center (MaRC::NaN)
{
}
