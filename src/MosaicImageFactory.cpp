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

#include <stdexcept>


MaRC::MosaicImageFactory::MosaicImageFactory (
    list_type && factories,
    MosaicImage::average_type type)
    : SourceImageFactory()
    , factories_(std::move(factories))
    , average_type_(type)
{
    if (this->factories_.empty())
        throw std::invalid_argument("Empty PhotoImageFactory list.");
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

    return
        std::make_unique<MosaicImage>(std::move(photos),
                                      this->average_type_);
}
