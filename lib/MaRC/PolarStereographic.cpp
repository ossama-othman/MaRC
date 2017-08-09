// $Id: PolarStereographic.cpp,v 1.4 2004/11/03 07:21:01 othman Exp $

#include "MaRC/PolarStereographic.h"
#include "MaRC/Constants.h"
#include "MaRC/OblateSpheroid.h"
#include "MaRC/root_find.h"

#include <limits>
#include <cmath>

template <typename T>
MaRC::PolarStereographic<T>::PolarStereographic (
   ValuePtr<OblateSpheroid> const & body,
   double max_lat,
   bool north_pole)
  : MapFactory<T> ()
  , body_ (body)
  , max_lat_ (isnan (max_lat)
              ? 0
              : (north_pole ? max_lat : -max_lat) * C::degree)
  , rho_coeff_ (2 * body->eq_rad () *
                ::pow (1 + body->first_eccentricity (),
                       -0.5 * (1 - body->first_eccentricity ())) *
                ::pow (1 - body->first_eccentricity (),
                       -0.5 * (1 + body->first_eccentricity ())))
  , distortion_coeff_ (::pow ((1 + body->first_eccentricity ()),
                              (1 - 2 * body->first_eccentricity ())) *
                       ::pow ((1 - body->first_eccentricity ()),
                              (1 + 2 * body->first_eccentricity ())) / 4 /
                       body->eq_rad () / body->eq_rad ())
  , north_pole_ (north_pole)
{
  if (!isnan (max_lat) && ::fabs (max_lat) >= 90)
    {
      std::ostringstream s;
      s << "Maximum polar stereographic latitude (" << max_lat << ") >= 90.";

      throw std::out_of_range (s.str ());
    }
}

template <typename T>
MaRC::PolarStereographic<T>::~PolarStereographic (void)
{
}

template <typename T>
MaRC::MapFactory<T> *
MaRC::PolarStereographic<T>::clone (void) const
{
  // Simple copy construction will suffice.
  return new MaRC::PolarStereographic<T> (*this);
}

template <typename T>
char const *
MaRC::PolarStereographic<T>::projection_name (void) const
{
  static char const name[] = "Polar Stereographic (Conformal)";

  return name;
}

template <typename T>
typename MaRC::PolarStereographic<T>::map_type *
MaRC::PolarStereographic<T>::make_map (SourceImage & source,
                                       unsigned int samples,
                                       unsigned int lines,
                                       double minimum,
                                       double maximum)
{
  std::auto_ptr<map_type> map (new map_type (samples, lines));

  unsigned int const nelem = samples * lines;

  unsigned int offset = 0;

  double const rho_max =
    this->stereo_rho (this->body_->graphic_latitude (this->max_lat_));
  double const min_dim = std::min (samples, lines);
  double const pix_conv_val = 2 * rho_max / min_dim;

  // Whether longitudes increase counter-clockwise in map.
  bool const ccw =
    ((this->north_pole_ && this->body_->prograde ())
     || (!this->north_pole_ && !this->body_->prograde ()));

  for (unsigned int k = 0; k < lines; ++k)
    {
      double const X = k + 0.5 - lines / 2.0;

      for (unsigned int i = 0; i < samples; ++i, ++offset)
        {
          double const Y = i + 0.5 - samples / 2.0;
          double const rho = pix_conv_val * ::sqrt (Y * Y + X * X);

//           if (rho > rho_max)
//             continue;

          double const old_latg =
            C::pi_2 - 2 * ::atan (rho / 2 / this->body_->eq_rad ());

          double const old_rho = this->stereo_rho (old_latg);

          // bodyGRAPHIC latitude.
          double const latg =
            MaRC::root_find (rho,
                             old_rho,
                             old_latg,
                             &PolarStereographic<T>::stereo_rho,
                             this);

          // Convert to bodyCENTRIC latitude
          double const lat =
            this->body_->centric_latitude (this->north_pole_ ? latg : -latg);

          double const lon =
            ::atan2 ((ccw ? Y : -Y), X);

          unsigned char const percent_complete =
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
typename MaRC::PolarStereographic<T>::grid_type *
MaRC::PolarStereographic<T>::make_grid (unsigned int samples,
                                        unsigned int lines,
                                        float lat_interval,
                                        float lon_interval)
{
  std::auto_ptr<grid_type> grid (new grid_type (samples, lines));

  static unsigned int const imax = 2000;

  double const rho_max =
    this->stereo_rho (this->body_->graphic_latitude (this->max_lat_));
  double const min_dim = std::min (samples - 1, lines - 1);
  double const pix_conv_val = 2 * rho_max / min_dim;

  static const typename grid_type::data_type white =
    std::numeric_limits<typename grid_type::data_type>::max ();

  // Draw latitude lines
  for (float n = -90 + lat_interval; n < 90; n += lat_interval)
    {
      // Convert to bodygraphic latitude
      double const nn = this->body_->graphic_latitude (n * C::degree);

      for (unsigned int m = 0; m < imax; ++m)
        {
          double const mm = static_cast<double> (m) / imax * C::degree * 360;

          double const z = this->stereo_rho (nn) * ::cos (mm);
          double const x = this->stereo_rho (nn) * ::sin (mm);

//           if (z > rho_max || x > rho_max)
//             continue;

          double const k = ::rint (z / pix_conv_val + lines / 2.0);
          double const i = ::rint (x / pix_conv_val + samples / 2.0);

          if (i >= 0 && i < static_cast<double> (samples)
              && k >= 0 && k < static_cast<double> (lines))
            {
              unsigned int const offset =
                static_cast<unsigned int> (k) * samples;

              (*grid)[offset + static_cast<unsigned int> (i)] = white;
            }
        }
    }

  // Draw longitude lines.
  for (float m = 360; m > 0; m -= lon_interval)
    {
      double const mm = m * C::degree;  // Convert to radians

      for (unsigned int n = 0; n < imax; ++n)
        {
          double const nn = static_cast<double> (n) / imax * C::degree * 360;

          double const z = this->stereo_rho (nn) * ::cos (mm);
          double const x = this->stereo_rho (nn) * ::sin (mm);

//           if (z > rho_max || x > rho_max)
//             continue;

          double const k = ::rint (z / pix_conv_val + lines / 2.0);
          double const i = ::rint (x / pix_conv_val + samples / 2.0);

          if (i >= 0 && i < static_cast<double> (samples)
              && k >= 0 && k < static_cast<double> (lines))
            {
              unsigned int const offset =
                static_cast<unsigned int> (k) * samples;

              (*grid)[offset + static_cast<unsigned int> (i)] = white;
            }
        }
    }

  return grid.release ();
}

template <typename T>
double
MaRC::PolarStereographic<T>::stereo_rho (double latg) const
{
  double const rho =
    this->rho_coeff_ * ::tan (C::pi_4 - latg / 2) *
    ::pow ((1 + this->body_->first_eccentricity () * ::sin (latg)) /
           (1 - this->body_->first_eccentricity () * ::sin (latg)),
           this->body_->first_eccentricity () / 2);

  return rho;
}

template <typename T>
double
MaRC::PolarStereographic<T>::distortion (double latg) const
{
  // Note that latitude is bodyGRAPHIC.
  return 1 + distortion_coeff_ * ::pow (this->stereo_rho (latg), 2.0);
}
