// -*- C++ -*-
/**
 * @file details/format.h
 *
 * @brief %MaRC convenience wrapper around fmtlib.
 *
 * Copyright (C) 2020  Ossama Othman
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * @author Ossama Othman
 *
 * @attention This is in an internal header that is not part of the
 *            public %MaRC API.
 */

#ifndef MARC_DETAILS_FORMAT_H
#define MARC_DETAILS_FORMAT_H

/// Avoid having to link the fmt library.
#ifndef FMT_HEADER_ONLY
# define FMT_HEADER_ONLY
#endif

#include <fmt/format.h>

#endif  // MARC_DETAILS_FORMAT_H
