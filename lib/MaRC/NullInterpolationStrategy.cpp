#include "NullInterpolationStrategy.h"


MaRC::NullInterpolationStrategy::~NullInterpolationStrategy (void)
{
}

bool
MaRC::NullInterpolationStrategy::interpolate (const double *,
                                              double,
                                              double,
                                              double &) const
{
  return true;
}

MaRC::InterpolationStrategy *
MaRC::NullInterpolationStrategy::clone (void) const
{
  // Stateless so a default constructed instance will suffice.
  return new NullInterpolationStrategy;
}
