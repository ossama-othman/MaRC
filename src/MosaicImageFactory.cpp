/**
 * @file MosaicImageFactory.cpp
 *
 * Copyright (C) 2004, 2017, 2020  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#include "MosaicImageFactory.h"

#include <marc/first_read.h>
#include <marc/unweighted_average.h>
#include <marc/weighted_average.h>

#include <stdexcept>


namespace
{
    std::unique_ptr<MaRC::compositing_strategy>
    make_compositor(MaRC::MosaicImageFactory::average_type type)
    {
        /**
         * @todo Drop the hard coding.  Make mosaic compositor
         *       selection extensible, such as through a compositor
         *       abstract factory.
         */
        switch (type) {
        case MaRC::MosaicImageFactory::AVG_NONE:
        default:
            return std::make_unique<MaRC::first_read>();
        case MaRC::MosaicImageFactory::AVG_UNWEIGHTED:
            return std::make_unique<MaRC::unweighted_average>();
        case MaRC::MosaicImageFactory::AVG_WEIGHTED:
            return std::make_unique<MaRC::weighted_average>();
        }
    }
}

// ----------------------------------------------------------------

MaRC::MosaicImageFactory::MosaicImageFactory (
    list_type && factories,
    average_type type)
    : SourceImageFactory()
    , factories_(std::move(factories))
    , average_type_(type)
{
    // No need to mosaic a single image.  This also covers the empty
    // factory list case.
    if (this->factories_.size() < 2)
        throw std::invalid_argument(
            "MosaicImageFactory used for less than two images.");
}

bool
MaRC::MosaicImageFactory::populate_parameters(
    MaRC::map_parameters &p) const
{
    /**
     * @todo Verify this achieves the desired result since all photos
     *       in this mosaic will plotted to the same map plane, rather
     *       than different planes.
     */
    for (auto const & image : this->factories_)
        if (!image->populate_parameters(p))
            return false;

    return true;
}

std::unique_ptr<MaRC::SourceImage>
MaRC::MosaicImageFactory::make(scale_offset_functor calc_so)
{
    /**
     * @todo Verify that the extrema handling in this method is
     *       correct.
     */

    extrema_type ex;
    bool valid_minimum = true;
    bool valid_maximum = true;
    MosaicImage::list_type photos;

    for (auto & factory : this->factories_) {
        auto const & minmax = factory->minmax();

        if (!minmax.minimum())
            valid_minimum = false;
        if (!minmax.maximum())
            valid_maximum = false;

        ex.update(minmax);

        photos.push_back(factory->make(calc_so));
    }

    /*
      Only set the mosaic image extrema if all photos in the mosaic
      have set extrema (e.g. FITS DATAMIN and/or DATAMAX values) to
      prevent inadvertently blocking out data from photos that don't
      set extrema values.  Previously set values, e.g. user-specified,
      will not be overridden.
    */
    if (valid_minimum)
        this->minimum(*ex.minimum());
    if (valid_maximum)
        this->maximum(*ex.maximum());

    auto compositor = make_compositor(this->average_type_);

    return
        std::make_unique<MosaicImage>(std::move(photos),
                                      std::move(compositor));
}
