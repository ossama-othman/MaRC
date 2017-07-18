// $Id: PhotoInterpolationStrategy.cpp,v 1.5 2005/11/09 18:58:57 othman Exp $

#include "PhotoInterpolationStrategy.h"

#include <cmath>

#include <iostream>


MaRC::PhotoInterpolationStrategy::~PhotoInterpolationStrategy (void)
{
}

MaRC::PhotoInterpolationStrategy::PhotoInterpolationStrategy (
  unsigned int samples,
  unsigned int lines,
  unsigned int nibble_left,
  unsigned int nibble_right,
  unsigned int nibble_top,
  unsigned int nibble_bottom)
  : samples_ (samples),
    left_ (nibble_left),
    right_ (samples - nibble_right),
    top_ (nibble_top),
    bottom_ (lines - nibble_bottom)
{
}

bool
MaRC::PhotoInterpolationStrategy::interpolate (const double * image,
                                               double x,
                                               double z,
                                               double & data) const
{
  // Bilinear interpolation over 2x2 area of pixels.

  const unsigned int l = static_cast<unsigned int> (x); // floor(x)
  const unsigned int r = l + 1;                         // ceil  (x);
  const unsigned int b = static_cast<unsigned int> (z); // floor(z)
  const unsigned int t = b + 1;                         // ceil  (z);

  // Offsets
  const unsigned int ob = b * this->samples_;
  const unsigned int ot = t * this->samples_;

  // Note that we assume the image is inverted from top to bottom.

  // e.g., l > 0 && r < samples && b > 0 && < t < lines
  if (l >= this->left_ && r < this->right_
      && b >= this->top_  && t < this->bottom_)
    {
      unsigned int count = 0;
      double tmp = 0;

      if (!std::isnan (image[ob + r]) && !std::isnan (image[ob + l]))
        {
          // [0][0]
          tmp += (image[ob + r] - image[ob + l]) * (x - l) + image[ob + l];

          // [1][1] =
          tmp += (image[ob + r] - image[ob + l]) * (z - b) + image[ob + r];

          count += 2;
        }

      if (!std::isnan (image[ot + r]) && !std::isnan (image[ot + l]))
        {
          // [0][1]
          tmp += (image[ot + r] - image[ot + l]) * (x - l) + image[ot + l];

          ++count;
        }

      if (!std::isnan (image[ot + l]) && !std::isnan (image[ob + l]))
        {
          // [1][0]
          tmp += (image[ot + l] - image[ob + l]) * (z - b) + image[ob + l];

          ++count;
        }

      if (count > 0)
        {
          data = tmp / count;

          return true;
        }
    }

  return false;
}

MaRC::InterpolationStrategy *
MaRC::PhotoInterpolationStrategy::clone (void) const
{
  // A copy constructed instance will suffice.
  return new PhotoInterpolationStrategy (*this);
}
