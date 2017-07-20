// -*- C++ -*-

//==========================================================================
/**
 *  @file NullInterpolationStrategy.h
 *
 *  @author Ossama Othman
 */
//==========================================================================


#ifndef MARC_NULL_INTERPOLATION_STRATEGY_H
#define MARC_NULL_INTERPOLATION_STRATEGY_H

#include "InterpolationStrategy.h"

namespace MaRC
{

  /**
   * @class NullInterpolationStrategy
   *
   * @brief Null (no-op) interpolation strategy.
   *
   * This interpolation strategy is a no-op.  It performs no
   * interpolation.
   */
  class NullInterpolationStrategy : public InterpolationStrategy
  {
  public:

    /// Destructor.
    virtual ~NullInterpolationStrategy (void);

    /// Performs no interpolation.
    virtual bool interpolate (const double *,
                              double,
                              double,
                              double &) const;

    /// Copy this NullInterpolationStrategy object.
    virtual InterpolationStrategy * clone (void) const;

  };

}

#endif  /* MARC_NULL_INTERPOLATION_STRATEGY_H */
