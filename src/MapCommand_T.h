// -*- C++ -*-

//==========================================================================
/**
 *  @file MapCommand_T.h
 *
 *  @author Ossama Othman
 */
//==========================================================================


#ifndef MARC_MAP_COMMAND_T_H
#define MARC_MAP_COMMAND_T_H

#include "MapCommand.h"

#include <MaRC/MapFactory.h>

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
    MapCommand_T(std::string const & filename,
                 std::string const & body_name,
                 std::unique_ptr<MapFactory<T>> factory,
                 long samples,
                 long lines);

    /**
     * @name @c MapCommand Methods.
     *
     * Methods required by the @c MapCommand interface.
     */
    //@{
    virtual const char * projection_name (void) const;
    virtual void initialize_FITS_image (fitsfile * fptr, int & status);
    virtual void make_map_planes (fitsfile * fptr, int & status);
    virtual Grid * make_grid(long samples,
                             long lines,
                             float lat_interval,
                             float lon_interval);
    //@}

  private:

      /// @c MapFactory object responsible for creating maps and
      /// grids.
      std::unique_ptr<MapFactory<T>> factory_;

  };

}

#include "MapCommand_T.cpp"

#endif  /* MARC_MAP_COMMAND_T_H */
