//==========================================================================
/**
 *  @file Version.cpp
 *
 *  MaRC library version information.
 *
 *  @author Ossama Othman
 */
//==========================================================================

#include "Version.h"
#include "config.h"


char const * MaRC::library_version()
{
    return PACKAGE_VERSION;
}
