// $Id: MosaicImageFactory.cpp,v 1.1 2004/07/03 10:59:42 othman Exp $

#include "MosaicImageFactory.h"


MaRC::MosaicImageFactory::MosaicImageFactory (const list_type & factories,
                                              MosaicImage::average_type type)
  : factories_ (factories),
    average_type_ (type)
{
}

MaRC::SourceImage *
MaRC::MosaicImageFactory::make (void)
{
  MosaicImage::list_type photos;

  list_type::const_iterator end = this->factories_.end ();
  for (list_type::iterator i = this->factories_.begin (); i != end; ++i)
    {
      std::auto_ptr<MaRC::PhotoImageFactory::return_type>
        source ((*i).make ());

      const PhotoImage & photo =
#ifndef MARC_HAS_COVARIANT_RETURN_TYPES
        dynamic_cast<PhotoImage &>
#endif  /* MARC_HAS_COVARIANT_RETURN_TYPES */
        (*source);

      photos.push_back (photo);
    }

  return new MosaicImage (photos, this->average_type_);
}

MaRC::ImageFactory *
MaRC::MosaicImageFactory::clone (void) const
{
  // Simple copy construction will suffice.
  return new MosaicImageFactory (*this);
}
