#include "MosaicImage.h"

#include <cmath>


MaRC::MosaicImage::MosaicImage (const list_type & images,
                                average_type type)
  : images_ (images),
    average_type_ (images.size () < 2 ? AVG_NONE : type)
{
  // Note that we override and disable averaging if only one image
  // exists in the image set.  There is no point in attempting
  // averaging in that case.
}

bool
MaRC::MosaicImage::read_data (const double & lat,
                              const double & lon,
                              double & data) const
{
  bool found_data = false;

  // Buffer holding weighted sum for a given pixel.
  long double weighted_data_sum = 0;

  // Sum of shortest distances.
  unsigned long long weight_sum = 0;

  const list_type::const_iterator begin = this->images_.begin ();
  const list_type::const_iterator end   = this->images_.end ();

  for (list_type::const_iterator i = begin; i != end; ++i)
    {
      switch (this->average_type_)
        {
        case AVG_WEIGHTED:   // Weighted averaging.
          {
            // Shortest distance to an edge of the source image or a
            // blank value in the source image.  This is used as a
            // weight when using weighted averaging.
            unsigned int weight = 1;

            static bool const scan = true; // Scan for data weight.

            if ((*i).read_data (lat,
                                lon,
                                data,
                                weight,
                                scan)
                && !this->is_zero (data))
              {
                weighted_data_sum += weight * data;
                weight_sum += weight;

                found_data = true;
              }
          }

          break;

        case AVG_UNWEIGHTED:  // Unweighted averaging.
          {
            if ((*i).read_data (lat, lon, data)
                && !this->is_zero (data))
              {
                weighted_data_sum += data;
                ++weight_sum;

                found_data = true;
              }
          }

          break;

        default:      // No averaging
          {
            if ((*i).read_data (lat, lon, data)
                && !this->is_zero (data))
              {
                found_data = true;

                // No point in continuing.  Just return the first
                // found datum.
                return found_data;
              }
          }

          break;
        }
    }

  // Perform the average.
  if (weight_sum > 0)
    data = static_cast<double> (weighted_data_sum / weight_sum);

  return found_data;
}

bool
MaRC::MosaicImage::is_zero (double data)
{
  // Only non-zero data is considered valid data.  This is
  // particularly necessary to avoid potential contribution of zero
  // data to data averages when creating a mosaic.

  // Anything less than this value is considered to be zero.
  static double const zero_threshold = 1e-10;

  return (fabs (data) < zero_threshold);
}
