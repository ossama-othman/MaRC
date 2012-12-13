// $Id: Data_Type_Test.cpp,v 1.1 2004/11/06 00:30:11 othman Exp $

// Test that verifies types of data potentially written to a FITS file
// have the expected sizes.

#include "MaRC/Long.h"

int
main ()
{
  return ((sizeof (unsigned char) == 1  // By definition, always 1!
           && sizeof (short)      == 2
           && sizeof (MaRC::Long) == 4
           && sizeof (float)      == 4
           && sizeof (double)     == 8)
          ? 0 : -1);
}
