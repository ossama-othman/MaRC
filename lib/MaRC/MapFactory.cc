// $Id: MapFactory.cc,v 1.6 2004/10/14 03:57:43 othman Exp $

#include "MapFactory.h"
#include "Map_traits.h"
#include "SourceImage.h"

#include <stdexcept>
#include <iostream>


template <typename T>
MaRC::MapFactory<T>::MapFactory (void)
  : percent_complete_old_ (0)
{
}

template <typename T>
MaRC::MapFactory<T>::~MapFactory (void)
{
}

template <typename T>
bool
MaRC::MapFactory<T>::plot (SourceImage & source,
                           double lat,
                           double lon,
                           double minimum,
                           double maximum,
                           unsigned char percent_complete,
                           double & data)
{
  // Clip minimum, maximum and datum to fit within map data type range, if
  // necessary.
  bool const found_data =
    (source.read_data (lat, lon, data)
     && data > Map_traits<T>::minimum (minimum)
     && data < Map_traits<T>::maximum (maximum));

//   std::cout << "found_data = " << found_data << std::endl
//             << "data = " << data << std::endl
//             << "minimum = " << Map_traits<T>::minimum (minimum) << std::endl
//             << "maximum = " << Map_traits<T>::maximum (maximum) << std::endl
//             << std::endl;

  /**
   * @todo Remove map progress output.  Library calls should not
   *       provide output unless desired by the user.
   */

  // Provide some output on the mapping progress.
  if (percent_complete == 100 && this->percent_complete_old_ != 0)
    {
      std::cout << "100%" << std::endl;
      this->percent_complete_old_ = 0;
    }
  else if (percent_complete > this->percent_complete_old_)
    {
      if (percent_complete % 20 == 0)
        std::cout
          << static_cast<unsigned int> (percent_complete) << std::flush;
      else if (percent_complete % 2 == 0)
        std::cout << "." << std::flush;

      this->percent_complete_old_ = percent_complete;
    }

  return found_data;
}
