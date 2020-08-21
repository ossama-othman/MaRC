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

#include <marc/config.h>  // FMT_HEADER_ONLY.

// See https://github.com/fmtlib/fmt/issues/947
#ifdef __GNUG__
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif

#include <fmt/format.h>

#ifdef __GNUG__
# pragma GCC diagnostic pop
#endif

#endif  // MARC_DETAILS_FORMAT_H
