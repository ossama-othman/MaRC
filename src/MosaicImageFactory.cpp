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
    MaRC::MapParameters &p) const
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
    extrema_type ex;
    bool valid_extrema = true;
    MosaicImage::list_type photos;

    /*
      Only set the mosaic image extrema if all photos in the mosaic
      have set extrema (e.g. FITS DATAMIN and DATAMAX values) to
      prevent inadvertently blocking out data from photos that don't
      set extrema values.  Avoid overriding previously set values, as
      well.
    */
    if (valid_extrema && !this->extrema_.is_valid())
        this->extrema_.update(ex);

    for (auto & factory : this->factories_) {
        auto const & minmax = factory->minmax();

        ex.update(minmax);

        if (!minmax.is_valid())
            valid_extrema = false;

        photos.push_back(factory->make(calc_so));
    }

    /*
      Only set the mosaic image extrema if all photos in the mosaic
      have set extrema (e.g. FITS DATAMIN and DATAMAX values) to
      prevent inadvertently blocking out data from photos that don't
      set extrema values.  Avoid overriding previously set values, as
      well.
    */
    if (valid_extrema && !this->extrema_.is_valid())
        this->extrema_.update(ex);

    return
        std::make_unique<MosaicImage>(std::move(photos),
                                      this->average_type_);
}
