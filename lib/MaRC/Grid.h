// -*- C++ -*-

//==========================================================================
/**
 *  @file Grid.h
 *
 *  @author Ossama Othman
 */
//==========================================================================


#ifndef MARC_GRID_H
#define MARC_GRID_H

#include <MaRC/Image.h>

namespace MaRC
{
  typedef Image<unsigned char> GridParent;

  /**
   * @class Grid
   *
   * @brief Representation of a generated grid image.
   *
   * This class merely encapsulates a generated grid image, and
   * exposes a simple interface that includes data element access
   * operators.  It also eases memory management of the underlying
   * data array.
   */
  class Grid : public GridParent
  {
  public:

    typedef GridParent::data_type data_type;

    /// Constructor.
    Grid (unsigned int samples, unsigned int lines);

  };

}

#endif
