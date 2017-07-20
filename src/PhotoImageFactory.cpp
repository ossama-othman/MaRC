#include "PhotoImageFactory.h"

#include "MaRC/PhotoImage.h"
#include "MaRC/GLLGeometricCorrection.h"
#include "MaRC/NullGeometricCorrection.h"
#include "MaRC/NaN.h"

#include <fitsio.h>

#include <stdexcept>
#include <cmath>
#include <sstream>
#include <memory>


MaRC::PhotoImageFactory::PhotoImageFactory (const char * filename,
                                            const OblateSpheroid & body)
  : filename_ (filename),
    flat_field_ (),
    body_ (body),
    geometric_correction_ (false),
    photometric_correction_ (false),
    sub_observ_lat_ (0),
    sub_observ_lon_ (0),
    sub_solar_lat_ (0),
    sub_solar_lon_ (0),
    range_ (-1),
    position_angle_ (0),
    arcsec_per_pixel_ (0),
    km_per_pixel_ (-1),
    focal_length_ (-1),
    scale_ (-1),
    OA_s_ (MaRC::NaN),
    OA_l_ (MaRC::NaN),
    nibble_left_ (0),
    nibble_right_ (0),
    nibble_top_ (0),
    nibble_bottom_ (0),
    sample_center_ (0),
    line_center_ (0),
    lat_at_center_ (MaRC::NaN),
    lon_at_center_ (MaRC::NaN),
    emi_ang_limit_ (-1),
    remove_sky_ (true),
    interpolate_ (false),
    use_terminator_ (true),
    invert_v_ (false),
    invert_h_ (false)
{
}

MaRC::PhotoImageFactory::return_type *
MaRC::PhotoImageFactory::make (void)
{
  fitsfile * fptr = 0;
  static const int mode = READONLY;
  int status = 0;  // CFITSIO requires initialization of status
                   // variable.

  (void) fits_open_image (&fptr, this->filename_.c_str (), mode, &status);

  if (status != 0)
    {
      // Report any errors before creating the map since map creation can
      // be time consuming.
      fits_report_error (stderr, status);

      std::ostringstream s;
      s << "Unable to open image \"" << this->filename_ << "\"";

      throw std::invalid_argument (s.str ());
    }

  int naxis = 0;
  static const unsigned int MAXDIM = 2;
  long naxes[MAXDIM] = { 0 };
  int bitpix = 0;
  (void) fits_get_img_param (fptr,
                             MAXDIM,
                             &bitpix,
                             &naxis,
                             naxes,
                             &status);

  const long nelements = naxes[0] * naxes[1];

  // Note that we're only reading a 2-dimensional image
  // above.
  double * img = new double[nelements];

  long fpixel[MAXDIM];
  fpixel[0] = 1;
  fpixel[1] = 1;

  double nulval = MaRC::NaN;
  int anynul;  // Unused

  (void) fits_read_pix (fptr,
                        TDOUBLE, // Array of type "double".
                        fpixel,
                        nelements,
                        &nulval,
                        img,
                        &anynul,
                        &status);

  fits_report_error (stderr, status);

  (void) fits_close_file (fptr, &status);

  // Perform flat fielding if a flat field file was provided.
  double * f_img = 0;
  if (this->flat_field_.length () != 0)
    {
      fitsfile * f_fptr = 0;
      (void) fits_open_image (&f_fptr,
                              this->flat_field_.c_str (),
                              mode,
                              &status);

      if (status != 0)
        {
          // Report any errors before creating the map since map creation can
          // be time consuming.
          fits_report_error (stderr, status);

          std::ostringstream s;
          s << "Unable to open flat field image \"" << this->filename_ << "\"";

          throw std::invalid_argument (s.str ());
        }

      int f_naxis = 0;
      long f_naxes[MAXDIM] = { 0 };
      int f_bitpix = 0;
      (void) fits_get_img_param (f_fptr,
                                 MAXDIM,
                                 &f_bitpix,
                                 &f_naxis,
                                 f_naxes,
                                 &status);

      // Verify flat field image is same size as source photo
      // image.
      if (f_naxes[0] != naxes[0] && f_naxes[1] != naxes[1])
        {
          (void) fits_close_file (fptr, &status);
          delete [] img;

          std::ostringstream s;
          s << "Mismatched source (" << naxes[0] << "x" << naxes[1] << ")"
            << " and flat field image "
            << "(" << f_naxes[0] << "x" << f_naxes[1] << ")" << "dimensions";

          throw std::runtime_error (s.str ());
        }

      // Note that we're only reading a 2-dimensional image
      // above.
      f_img = new double[nelements];

      double nulval = MaRC::NaN;
      int anynul;  // Unused

      (void) fits_read_pix (fptr,
                            TDOUBLE, // Array of type "double".
                            fpixel,
                            nelements,
                            &nulval,
                            f_img,
                            &anynul,
                            &status);

      fits_report_error (stderr, status);

      (void) fits_close_file (fptr, &status);
    }

  if (status != 0)
    {
      delete [] f_img;
      delete [] img;

      char fits_error[30] = { 0 };
      fits_get_errstatus (status, fits_error);

      throw std::runtime_error (fits_error);
    }

  // Perform flat fielding if desired.
  if (f_img != 0)
    for (long i = 0; i < nelements; ++i)
      img[i] -= f_img[i];

  delete [] f_img; // No longer need the flat field image.

  // Invert image if desired.
  if (this->invert_h_)
    this->invert_h (img,
                    static_cast<unsigned int> (naxes[0]),
                    static_cast<unsigned int> (naxes[1]));

  if (this->invert_v_)
    this->invert_v (img,
                    static_cast<unsigned int> (naxes[0]),
                    static_cast<unsigned int> (naxes[1]));

  // Potential memory leak here. If the below allocation
  // fails, "img" will be leaked since it is raw C++ array.

  std::unique_ptr<MaRC::GeometricCorrection> gc;
  if (this->geometric_correction_)
    {
      MaRC::GeometricCorrection * gll =
        new MaRC::GLLGeometricCorrection (
          static_cast<unsigned int> (naxes[0]) /* samples */);

      gc.reset (gll);
    }
  else
    {
      MaRC::GeometricCorrection * nullcor =
        new MaRC::NullGeometricCorrection;

      gc.reset (nullcor);
    }

  std::unique_ptr<PhotoImage> photo (
    new MaRC::PhotoImage (this->body_,
                          img,
                          static_cast<unsigned int> (naxes[0]), // samples
                          static_cast<unsigned int> (naxes[1]), // lines
                          gc.get ()));

  (void) gc.release (); // Don't need to hold on to it any more.

  photo->nibble_left   (this->nibble_left_);
  photo->nibble_right  (this->nibble_right_);
  photo->nibble_top    (this->nibble_top_);
  photo->nibble_bottom (this->nibble_bottom_);

  if (this->km_per_pixel_ > 0)
    photo->km_per_pixel (this->km_per_pixel_);

  if (this->arcsec_per_pixel_ > 0)
    photo->arcsec_per_pixel (this->arcsec_per_pixel_);

  if (this->focal_length_ > 0)
    photo->focal_length (this->focal_length_);

  if (this->scale_ > 0)
    photo->scale (this->scale_);

  if (!std::isnan (this->OA_s_))
    photo->optical_axis_sample (this->OA_s_);

  if (!std::isnan (this->OA_l_))
    photo->optical_axis_line (this->OA_l_);

  if (!std::isnan (this->sample_center_))
    photo->body_center_sample (this->sample_center_);

  if (!std::isnan (this->line_center_))
    photo->body_center_line (this->line_center_);

  if (!std::isnan (this->lat_at_center_))
    photo->lat_at_center (this->lat_at_center_);

  if (!std::isnan (this->lon_at_center_))
    photo->lon_at_center (this->lon_at_center_);

  if (this->emi_ang_limit_ > 0)
    photo->emi_ang_limit (this->emi_ang_limit_);

  photo->sub_observ (this->sub_observ_lat_, this->sub_observ_lon_);
  photo->position_angle (this->position_angle_);
  photo->sub_solar (this->sub_solar_lat_, this->sub_solar_lon_);
  photo->range (this->range_);

  photo->remove_sky (this->remove_sky_);
  photo->interpolate (this->interpolate_);
  photo->use_terminator (this->use_terminator_);

  photo->finalize_setup ();

  /**
   * @todo Improve exception safety.
   */
  return photo.release ();
}

MaRC::ImageFactory *
MaRC::PhotoImageFactory::clone (void) const
{
  // Simple copy construction will suffice.
  return new PhotoImageFactory (*this);
}

void
MaRC::PhotoImageFactory::filename (const char * name)
{
  this->filename_ = name;
}

void
MaRC::PhotoImageFactory::flat_field (const char * name)
{
  this->flat_field_ = name;
}

void
MaRC::PhotoImageFactory::nibbling (unsigned int left,
                                   unsigned int right,
                                   unsigned int top,
                                   unsigned int bottom)
{
  this->nibble_left_   = left;
  this->nibble_right_  = right;
  this->nibble_top_    = top;
  this->nibble_bottom_ = bottom;
}

void
MaRC::PhotoImageFactory::invert (bool vertical, bool horizontal)
{
  this->invert_v_ = vertical;
  this->invert_h_ = horizontal;
}

void
MaRC::PhotoImageFactory::interpolate (bool i)
{
  this->interpolate_ = i;
}

void
MaRC::PhotoImageFactory::remove_sky (bool r)
{
  this->remove_sky_ = r;
}

void
MaRC::PhotoImageFactory::emi_ang_limit (double angle)
{
  this->emi_ang_limit_ = angle;
}

void
MaRC::PhotoImageFactory::body_center (double sample, double line)
{
  this->sample_center_ = sample;
  this->line_center_   = line;
}

void
MaRC::PhotoImageFactory::lat_lon_center (double lat, double lon)
{
  this->lat_at_center_ = lat;
  this->lon_at_center_ = lon;
}

void
MaRC::PhotoImageFactory::optical_axis (double sample, double line)
{
  this->OA_s_ = sample;
  this->OA_l_ = line;
}

void
MaRC::PhotoImageFactory::geometric_correction (bool g)
{
  this->geometric_correction_ = g;
}

void
MaRC::PhotoImageFactory::photometric_correction (bool p)
{
  this->photometric_correction_ = p;
}

void
MaRC::PhotoImageFactory::sub_observ (double lat, double lon)
{
  this->sub_observ_lat_ = lat;
  this->sub_observ_lon_ = lon;
}

void
MaRC::PhotoImageFactory::sub_solar (double lat, double lon)
{
  this->sub_solar_lat_ = lat;
  this->sub_solar_lon_ = lon;
}

void
MaRC::PhotoImageFactory::range (double r)
{
  this->range_ = r;
}

void
MaRC::PhotoImageFactory::focal_length (double len)
{
  this->focal_length_ = len;
}

void
MaRC::PhotoImageFactory::scale (double s)
{
  this->scale_ = s;
}

void
MaRC::PhotoImageFactory::position_angle (double north)
{
  this->position_angle_ = north;
}

void
MaRC::PhotoImageFactory::arcsec_per_pixel (double a)
{
  this->arcsec_per_pixel_ = a;
}

void
MaRC::PhotoImageFactory::km_per_pixel (double k)
{
  this->km_per_pixel_ = k;
}

void
MaRC::PhotoImageFactory::use_terminator (bool u)
{
  this->use_terminator_ = u;
}

void
MaRC::PhotoImageFactory::invert_h (double * image,
                                   unsigned int samples,
                                   unsigned int lines)
{
// Invert image from left to right.

  const unsigned int middle = samples / 2;

  for (unsigned int line = 0; line < lines; ++line)
    {
      for (unsigned int sample = 0; sample < middle; ++sample)
        {
          const unsigned int offset = samples - sample - 1;

          const double temp = image[sample];

          image[sample] = image[offset];
          image[offset] = temp;
        }
    }
}

void
MaRC::PhotoImageFactory::invert_v (double * image,
                                   unsigned int samples,
                                   unsigned int lines)
{
  // Invert image from top to bottom.
  const unsigned int middle = lines / 2;

  for (unsigned int line = 0; line < middle; ++line)
    {
      const unsigned int offset  = (lines - line - 1) * samples;

      for (unsigned int sample = 0; sample < samples; ++sample)
        {
          const double temp = image[samples * line + sample];

          image[sample] = image[offset];
          image[offset] = temp;
        }
    }
}
