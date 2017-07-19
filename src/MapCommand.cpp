// $Id: MapCommand.cpp,v 1.8 2005/02/08 20:34:24 othman Exp $

#include "MapCommand.h"
#include "FITS_traits.h"

#include <MaRC/Grid.h>
#include <MaRC/config.h>

#include <fitsio.h>

#include <iostream>
#include <memory>

#include <unistd.h>


MaRC::MapCommand::MapCommand (std::string const & filename,
                              std::string const & body_name,
                              long samples,
                              long lines)
  : samples_ (samples),
    lines_ (lines),
    image_factories_ (),
    blank_set_ (false),
    blank_ (0),
    filename_ (filename),
    author_ (),
    origin_ (),
    comments_ (),
    xcomments_ (),
    body_name_ (body_name),
    lat_interval_ (0),
    lon_interval_ (0),
    bscale_ (1),
    bzero_ (0),
    transform_data_ (false),
    create_grid_ (false)
{
}

MaRC::MapCommand::~MapCommand (void)
{
}

int
MaRC::MapCommand::execute (void)
{
  std::cout << std::endl << "Creating map: " << this->filename_ << std::endl;

  (void) unlink (this->filename_.c_str ());

  // Create the map file.
  fitsfile * fptr = 0;
  int status = 0;
  fits_create_file (&fptr, this->filename_.c_str (), &status);

  // Create primary image array HDU.
  this->initialize_FITS_image (fptr, status);

  // Write the author name if supplied.
  if (this->author_.length () != 0)
    {
      const char * author = this->author_.c_str ();
      fits_update_key (fptr,
                       TSTRING,
                       "AUTHOR",
                       const_cast<char *> (author),
                       "Map creator",
                       &status);
    }

  // Write the name of the organization or institution responsible for
  // creating the FITS file, if supplied.
  if (this->origin_.length () != 0)
    {
      const char * origin = this->origin_.c_str ();
      fits_update_key (fptr,
                       TSTRING,
                       "ORIGIN",
                       const_cast<char *> (origin),
                       "Map creator organization",
                       &status);
    }

  // Write the current date and time (i.e. the creation time) into the
  // map FITS file.
  fits_write_date (fptr, &status);

  // Write the name of the object being mapped.
  const char * object = this->body_name_.c_str ();
  fits_update_key (fptr,
                   TSTRING,
                   "OBJECT",
                   const_cast<char *> (object),
                   "Name of observed object.",
                   &status);

  // Write the map comments.
  comment_list_type::iterator comment_end = this->comments_.end ();
  for (comment_list_type::iterator i = this->comments_.begin ();
       i != comment_end;
       ++i)
    {
      fits_write_comment (fptr, (*i).c_str (), &status);
    }

  std::string const history =
    std::string (this->projection_name ())
      + " projection created using MaRC " PACKAGE_VERSION ".";

  // Write some MaRC-specific HISTORY comments.
  fits_write_history (fptr,
                      history.c_str (),
                      &status);

  // Write the BSCALE and BZERO keywords and value into the map FITS file.
  if (this->transform_data_)
    {
      fits_update_key (fptr,
                       TFLOAT,
                       "BSCALE",
                       &this->bscale_,
                       "Coefficient of linear term in the scaling equation.",
                       &status);

      fits_update_key (fptr,
                       TFLOAT,
                       "BZERO",
                       &this->bzero_,
                       "Physical value corresponding to an array "
                       "value of zero.",
                       &status);
    }

  if (status != 0)
    {
      // Report any errors before creating the map since map creation can
      // be time consuming.
      fits_report_error (stderr, status);

      fits_close_file (fptr, &status);

      return status;
    }

  // Create and write the map planes.
  this->make_map_planes (fptr, status);

//   // Write DATAMIN and DATAMAX keywords.
//   fits_update_key (fptr,
//                    TFLOAT,
//                    "DATAMIN",
//                    &this->minimum_,
//                    "Minimum valid physical value.",
//                    &status);

//   fits_update_key (fptr,
//                    TFLOAT,
//                    "DATAMAX",
//                    &this->maximum_,
//                    "Maximum valid physical value.",
//                    &status);

  // Write a checksum for the map.
  fits_write_chksum (fptr, &status);

  if (this->create_grid_)
    {
      long naxes[] = { this->samples_, this->lines_ };
      constexpr int grid_naxis = sizeof(naxes) / sizeof(naxes[0]);

      static_assert(grid_naxis == 2,
                    "Map grid is not two dimensional.");

      // Create the image extension containing the map grid.
      /**
       * @todo Check return value!
       */
      fits_create_img(fptr,
                      FITS::traits<FITS::byte_type>::bitpix,
                      grid_naxis,
                      naxes,
                      &status);

      // Write the grid comments.
      comment_list_type::iterator xcomment_end = this->xcomments_.end ();
      for (comment_list_type::iterator i = this->xcomments_.begin ();
           i != xcomment_end;
           ++i)
        {
          fits_write_comment (fptr, (*i).c_str (), &status);
        }

      std::string const xhistory =
        std::string (this->projection_name ())
        + " projection grid created using MaRC " PACKAGE_VERSION ".";

      // Write some MaRC-specific HISTORY comments.
      fits_write_history (fptr,
                          xhistory.c_str (),
                          &status);

//       int grid_blank = 0;
//       fits_update_key (fptr,
//                        TINT,
//                        "BLANK",
//                        &grid_blank,
//                        "Value of off-grid pixels.",
//                        &status);

      std::unique_ptr<Grid> grid(this->make_grid(this->samples_,
                                                 this->lines_,
                                                 this->lat_interval_,
                                                 this->lon_interval_));

      // LONGLONG is a CFITSIO type.
      constexpr LONGLONG fpixel = 1;  // First pixel/element
      LONGLONG const nelements = this->samples_ * this->lines_;

      /**
       * @todo Check return value!
       */
      (void) fits_write_img(fptr,
                            FITS::traits<FITS::byte_type>::datatype,
                            fpixel,
                            nelements,
                            grid->get(),
                            &status);

      // Write a checksum for the grid.
      fits_write_chksum (fptr, &status);
    }

  fits_close_file (fptr, &status);

  fits_report_error (stderr, status);

  return status;
}

void
MaRC::MapCommand::author (const std::string & author)
{
  this->author_ = author;
}

void
MaRC::MapCommand::origin (const std::string & origin)
{
  this->origin_ = origin;
}

void
MaRC::MapCommand::comment_list (const comment_list_type & comments)
{
  this->comments_ = comments;
}

void
MaRC::MapCommand::xcomment_list (const comment_list_type & comments)
{
  this->xcomments_ = comments;
}

void
MaRC::MapCommand::grid_intervals (float lat_interval, float lon_interval)
{
  this->create_grid_ = true;
  this->lat_interval_ = lat_interval;
  this->lon_interval_ = lon_interval;
}

void
MaRC::MapCommand::data_zero (float zero)
{
  this->bzero_ = zero;
  this->transform_data_ = true;
}

void
MaRC::MapCommand::data_scale (float scale)
{
  this->bscale_ = scale;
  this->transform_data_ = true;
}

void
MaRC::MapCommand::data_blank (int blank)
{
  this->blank_ = blank;
  this->blank_set_ = true;
}

void
MaRC::MapCommand::image_factories (const image_factories_type & factories)
{
  this->image_factories_ = factories;
}
