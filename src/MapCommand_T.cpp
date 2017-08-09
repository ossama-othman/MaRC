// $Id: MapCommand_T.cpp,v 1.3 2004/07/06 07:21:33 othman Exp $

#include "MapCommand_T.h"
#include "FITS_traits.h"

#include <fitsio.h>


template <typename T>
MaRC::MapCommand_T<T>::MapCommand_T (const std::string & filename,
                                     const std::string & body_name,
                                     std::auto_ptr<MapFactory<T> > factory,
                                     unsigned int samples,
                                     unsigned int lines)
  : MapCommand (filename, body_name, samples, lines),
    factory_ (factory.release ())  // Non-throwing
{
}

template <typename T>
const char *
MaRC::MapCommand_T<T>::projection_name (void) const
{
  return this->factory_->projection_name ();
}

template <typename T>
void
MaRC::MapCommand_T<T>::initialize_FITS_image (fitsfile * fptr, int & status)
{
  const unsigned int planes = this->image_factories_.size ();

  const int naxis =
    (planes > 1
     ? 3 /* 3 dimensions -- map "cube" */
     : 2 /* 2 dimensions -- map "plane" */);

  // Specify map cube dimensions.  Note that in the two-dimensional
  // map case, the third "planes" dimension will be ignored since the
  // above "naxis" variable will be set to two, not three.
  long naxes[] = { this->samples_, this->lines_, planes };

  // Create the primary array.
  FITS_traits<T>::create_img (fptr, naxis, naxes, status);

  // Write the BLANK keyword and value into the map FITS file.
  if (FITS_traits<T>::supports_blank_keyword && this->blank_set_)
    {
      fits_update_key (fptr,
                       TINT,
                       "BLANK",
                       &this->blank_,
                       "Value of pixels with undefined physical value.",
                       &status);
    }
}

template <typename T>
void
MaRC::MapCommand_T<T>::make_map_planes (fitsfile * fptr, int & status)
{
  // First pixel/element in FITS array (1-based).
  //   Plane 1: fpixel =  1
  //   Plane 2: fpixel += nelements
  //   Plane 3: fpixel += nelements
  //   Plane 4: ... etc ...
  long fpixel = 1;

  const unsigned int nelements = this->samples_ * this->lines_;

  // Keep track of mapped planes for reporting to user.
  unsigned int plane_count = 1;
  const unsigned int num_planes = this->image_factories_.size ();

  // Create and write the map planes.
  const image_factories_type::const_iterator end =
    this->image_factories_.end ();
  for (image_factories_type::const_iterator i =
         this->image_factories_.begin ();
       i != end;
       ++i)
    {
      std::cout << "Plane "
                << plane_count << " / " << num_planes
                <<" : " << std::flush;

      // Create the SourceImage.
      std::auto_ptr<SourceImage> image ((*i)->make ());

      // Create the map plane.
      std::auto_ptr<typename MapFactory<T>::map_type> map (
        this->factory_->make_map (*image,
                                  this->samples_,
                                  this->lines_,
                                  (*i)->minimum (),
                                  (*i)->maximum ()));

      // Supersamping verification.
      image->check_image_unread_mask();

      FITS_traits<T>::write_img (fptr,
                                 fpixel,
                                 static_cast<long> (nelements),
                                 map->get (),
                                 status);

      // Set offset in the FITS array to the next plane.
      fpixel += static_cast<long> (nelements);

      ++plane_count;
    }
}

template <typename T>
MaRC::Grid *
MaRC::MapCommand_T<T>::make_grid (unsigned int samples,
                                  unsigned int lines,
                                  float lat_interval,
                                  float lon_interval)
{
  return this->factory_->make_grid (samples,
                                    lines,
                                    lat_interval,
                                    lon_interval);
}

template <typename T>
MaRC::MapCommand *
MaRC::MapCommand_T<T>::clone (void) const
{
  // Simple copy construction will suffice.
  return new MapCommand_T<T> (*this);
}
