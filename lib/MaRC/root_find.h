//   -*- C++ -*-

//==========================================================================
/**
 *  @file root_find.h
 *
 *  MaRC root finding related functions
 *
 *  @author Ossama Othman
 */
//==========================================================================

#ifndef MARC_ROOT_FIND_H
#define MARC_ROOT_FIND_H

#include <cmath>

namespace MaRC
{
  template <class MAP_FACTORY>
  double
  root_find (double ordinate,
             double ordinate_guess,
             double abscissa_guess,
             double (MAP_FACTORY::* equation)(double) const,
             MAP_FACTORY * map)
{
  double const tolerance = 1e-4;
  double const delta = 1e-3;
  unsigned int const max_iterations = 10;
  double const abscissa_guess_orig = abscissa_guess;

  double abscissa;

  for (unsigned int i = 0; i < max_iterations; ++i)
    {
      // Center divided difference numerical method of computing the
      // first derivative (VERY LOW ERROR)
      double const deriv =
        (-(map->*equation) (abscissa_guess + 2 * delta)
         + 8 * (map->*equation) (abscissa_guess + delta)
         - 8 * (map->*equation) (abscissa_guess - delta)
         + (map->*equation) (abscissa_guess - 2 * delta)) / 12 / delta;

      abscissa = abscissa_guess + (ordinate - ordinate_guess) / deriv;

      if (::fabs (abscissa - abscissa_guess) < tolerance)
        return abscissa;

      abscissa_guess = abscissa;
      ordinate_guess = (map->*equation) (abscissa_guess);
    }

  // Root not found.

  double const begin = abscissa_guess_orig - abscissa_guess_orig * 2;
  double const end   = abscissa_guess_orig + abscissa_guess_orig * 2;

  // Try an incremental search for a good initial guess since original guess
  // was not successful.
  abscissa_guess = begin;
  for (unsigned int count = 0; count < max_iterations; ++count)
    {
      double const temp = abscissa_guess;

      ordinate_guess = (map->*equation) (abscissa_guess);

      for (unsigned int i = 0; i < max_iterations; ++i)
        {
          // Center divided difference numerical method of computing
          // the first derivative (VERY LOW ERROR).
          double const deriv =
            (-(map->*equation) (abscissa_guess + 2 * delta)
             + 8 * (map->*equation) (abscissa_guess + delta)
             - 8 * (map->*equation) (abscissa_guess - delta)
             + (map->*equation) (abscissa_guess - 2 * delta)) / 12 / delta;

          abscissa = abscissa_guess + (ordinate - ordinate_guess) / deriv;

          if (::fabs (abscissa - abscissa_guess) < tolerance)
            return abscissa;

          abscissa_guess = abscissa;
          ordinate_guess = (map->*equation) (abscissa_guess);
        }

      abscissa_guess = temp;
      abscissa_guess += delta;

      if (abscissa_guess >= end)
        break;
    }

  throw
    std::runtime_error ("INTERNAL ERROR: Root finding process "
                        "seems to be diverging.");

  return abscissa;
}


}

#endif  /* MARC_ROOT_FIND_H */
