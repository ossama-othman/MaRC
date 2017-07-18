// $Id: NaN_Test.cpp,v 1.1 2004/11/06 00:30:12 othman Exp $

// Test that verifies the MaRC::NaN constant is actually
// "Not-A-Number".

#include <MaRC/NaN.h>

#include <cmath>

int
main ()
{
  return (std::isnan (MaRC::NaN) ? 0 : -1);
}
