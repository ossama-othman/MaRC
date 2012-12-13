// -*- C++ -*-

//==========================================================================
/**
 *  @file Long.h
 *
 *  $Id: Long.h,v 1.1 2004/10/11 06:20:53 othman Exp $
 *
 *  32-bit signed integer type definition.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================

#ifndef MARC_LONG_H
#define MARC_LONG_H

#include "MaRC/IfThenElse.h"


namespace MaRC
{

  /**
   * \typedef Long
   *
   * \brief Type definition for a signed 32 bit integer.
   *
   * This is the type definition for a signed 32 bit integer.  It is
   * actually determined at compile-time through partial template
   * specialization techniques.
   *
   * @note This \c typedef is mostly for the convenience of
   *       application code that uses the MaRC library, and requires a
   *       portable 32 bit integer \c typedef.  It is not used, nor is
   *       it needed within the MaRC library.
   *
   * @attention If no 32 bit integer is found, the type is intentionally
   *            defined to be \c void to prevent the \c typedef from
   *            being valid.  In that case, additional type choices
   *            must be added to the potential 32 bit integer cases.
   */
  typedef IfThenElse<(sizeof (int) == 4),
                     int,
                     IfThenElse<(sizeof (long) == 4),
                                long,
                                void>::result_type>::result_type Long;

}

#endif  /* MARC_LONG_H */
