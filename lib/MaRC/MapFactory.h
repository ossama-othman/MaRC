// -*- C++ -*-

//==========================================================================
/**
 *  @file MapFactory.h
 *
 *  @author Ossama Othman
 */
//==========================================================================


#ifndef MARC_MAP_FACTORY_H
#define MARC_MAP_FACTORY_H

#include <MaRC/Image.h>
#include <MaRC/Grid.h>

#include <MaRC/ValuePtr.h>

#include <limits>

namespace MaRC
{
  class SourceImage;

  /**
   * @class MapFactory
   *
   * @brief Map Abstract Factory
   *
   * Abstract Factory class for map projections supported by MaRC.
   */
  template <typename T>
  class MapFactory
  {
  public:

    /// @typedef Type of data to be stored in map.
    typedef T        data_type;

    /// @typedef Type returned from @c make_map() method.
    typedef Image<T> map_type;

    /// @typedef Type returned from @c make_grid() method.
    typedef Grid     grid_type;

    /// Constructor.
    MapFactory (void);

    /// Destructor.
    virtual ~MapFactory (void);

    /// Clone operation that polymorphically copies the concrete
    /// @c MapFactory object.
    /**
     * This clone operation is generally used in conjunction with the
     * VP_traits<MapFactory> template specialization to prevent the
     * slicing that occurs when copying through a copy constructor
     * instead.
     */
    virtual MapFactory<T> * clone (void) const = 0;

    /// Return the name of the map projection.
    virtual const char * projection_name (void) const = 0;

    /// Create the desired map projection.
    /**
     * @param source  SourceImage object containing the data to be
     *                mapped.
     * @param samples Number of samples in map.
     * @param lines   Number of lines   in map.
     * @param minimum Minimum allowed value map, i.e. all data \> minimum.
     * @param maximum Maximum allowed value map, i.e. all data \< maximum.
     */
    virtual map_type * make_map (
        const SourceImage & source,
        unsigned int samples,
        unsigned int lines,
        double minimum = -std::numeric_limits<double>::max (),
        double maximum =  std::numeric_limits<double>::max ()) = 0;

    /// Create the latitude/longitude grid for the desired map
    /// projection.
    /**
     * @param samples Number of samples in grid.
     * @param lines   Number of lines   in grid.
     * @param lat_interval Number of degrees between each latitude
     *                     grid line.
     * @param lon_interval Number of degrees between each longitude
     *                     grid line.
     */
    virtual grid_type * make_grid (unsigned int samples,
                                   unsigned int lines,
                                   float lat_interval,
                                   float lon_interval) = 0;

  protected:

    /// Plot the data on the map.
    /**
     * Plot the data at given latitude and longitude on the map.
     *
     * @param source           SourceImage object containing the data
     *                         to be mapped.
     * @param lat              Bodycentric latitude
     * @param lon              Bodycentric longitude
     * @param minimum          Minimum allowed value map, i.e. all
     *                         data \> minimum.
     * @param maximum          Maximum allowed value map, i.e. all
     *                         data \< maximum.
     * @param percent_complete Percent of map completed
     * @param[out] data        Data retrieved from source image.
     *
     * @return true if data was found at given latitude and longitude.
     */
    bool plot (const SourceImage & source,
               double lat,
               double lon,
               double minimum,
               double maximum,
               unsigned char percent_complete,
               double & data);

  private:

    /// Previously measured percentage of map completed.
    unsigned char percent_complete_old_;

  };

  // MapFactory specialization of the ValuePtr traits template.
  template<>
  template<typename T>
  struct VP_traits<MapFactory<T> >
  {
    static MapFactory<T> * clone (const MapFactory<T> * p)
    {
      return p->clone ();
    }
  };

}

#include "MaRC/MapFactory.cpp"

#endif
