#include "NaN.h"
#include "config.h"

#if defined (MARC_HAS_STL_QUIET_NAN_SUPPORT)

# include <limits>

double const MaRC::NaN =
  std::numeric_limits<double>::quiet_NaN ();

#elif defined (MARC_C_LIB_QUIET_NAN) && defined (_ISOC99_SOURCE)

# include <cmath>
# include <cstdlib>

double const MaRC::NaN = MARC_C_LIB_QUIET_NAN;

#else
# error MaRC requires quiet NaN support!
#endif
