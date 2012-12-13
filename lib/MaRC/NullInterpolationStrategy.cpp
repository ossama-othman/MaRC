// $Id: NullInterpolationStrategy.cpp,v 1.3 2005/11/09 18:58:57 othman Exp $

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
