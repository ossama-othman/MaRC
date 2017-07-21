// Test that verifies the MaRC::NaN constant is actually
// "Not-A-Number".

#include <MaRC/NaN.h>

#include <cmath>

int
main ()
{
  return (std::isnan (MaRC::NaN) ? 0 : -1);
}
