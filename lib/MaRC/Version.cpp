//==========================================================================
/**
 *  @file Version.cpp
 *
 * MaRC library version information.
 *
 *  @author Ossama Othman <ossama.othman@gmail.com>
 */
//==========================================================================

#include "Version.h"
#include "config.h"


char const * MaRC::library_version()
{
    return PACKAGE_VERSION;
}
