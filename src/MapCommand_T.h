// -*- C++ -*-

//==========================================================================
/**
 *  @file MapCommand_T.h
 *
 *  $Id: MapCommand_T.h,v 1.2 2004/07/06 00:01:41 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//==========================================================================


#ifndef MARC_MAP_COMMAND_T_H
#define MARC_MAP_COMMAND_T_H

#include "MapCommand.h"

#include "MaRC/ValuePtr.h"
#include "MaRC/MapFactory.h"

#include <memory>


namespace MaRC
{
  /**
   * @class MapCommand_T
   *
   * @brief Type-specific implementation of MapCommand.
   *
   * Implements type-specific MapCommand operations, such as map
   * creation, etc.  Note that the template type is only visible
   * within the implementation, not within the interface, meaning that
   * execution of this type-specific MapCommand may be invoked
   * polymorphically from a pointer or reference to the
   * non-templatized base MapCommand class.  This simplifies client
   * code that executes MapCommands.
   */
  template <typename T>
  class MapCommand_T : public MapCommand
  {
  public:

    /// Constructor.
    MapCommand_T (const std::string & filename,
                  const std::string & body_name,
                  std::unique_ptr<MapFactory<T> > factory,
                  unsigned int samples,
                  unsigned int lines);

    /**
     * @name @c MapCommand Methods.
     *
     * Methods required by the @c MapCommand interface.
     */
    //@{
    virtual const char * projection_name (void) const;
    virtual void initialize_FITS_image (fitsfile * fptr, int & status);
    virtual void make_map_planes (fitsfile * fptr, int & status);
    virtual Grid * make_grid (unsigned int samples,
                              unsigned int lines,
                              float lat_interval,
                              float lon_interval);
    virtual MapCommand * clone (void) const;
    //@}

  private:

    /// @c MapFactory object responsible for creating maps and grids.
    MaRC::ValuePtr<MapFactory<T> > factory_;

  };

}

#include "MapCommand_T.cpp"

#endif  /* MARC_MAP_COMMAND_T_H */
