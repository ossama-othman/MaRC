// -*- C++ -*-
/**
 * @file calc.cpp
 *
 * Copyright (C) 1996-1998, 2004, 2017  Ossama Othman
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * @author Ossama Othman
 */

#include "calc.h"
#include "parse_scan.h"
#include "parse.hh"  /* Parser definiitons */

#include <cmath>


// ---------------------------------------------------------------

MaRC::sym_entry::sym_entry(double (*fnct)(double))
    : type(FNCT)
{
    value.fnctptr = fnct;
}

MaRC::sym_entry::sym_entry(double var)
    : type(VAR)
{
    value.var = var;
}

// ---------------------------------------------------------------

MaRC::symrec::symrec()
    : table_()
{
    struct init
    {
        char const * const fname;
        double (*fnct)(double);
    };

    /**
     * @todo Add other useful functions like @c pow(), etc.
     */
    static init const arith_fncts[] =
    {
        { "sin",  sin  },
        { "cos",  cos  },
        { "tan",  tan  },
        { "asin", asin },
        { "acos", acos },
        { "atan", atan },
        { "ln",   log  },
        { "exp",  exp  },
        { "sqrt", sqrt }
    };

    // Pre-populate the symbol table with "built-in" functions.
    for (auto & i : arith_fncts)
        this->table_.emplace(std::make_pair(i.fname,
                                            sym_entry(i.fnct)));
}

void
MaRC::symrec::putsym(char const * name, int /* type */)
{
    /**
     * @todo Do we need the @a type argument? Isn't @a type always
     *       @c VAR?
     */

    // Use emplace() (or insert()) instead of operator[] to prevent
    // users from overwriting "built-in" function entries we added.
    this->table_.emplace(std::make_pair(name, sym_entry(0.0)));
}

MaRC::sym_entry *
MaRC::symrec::getsym(char const * name)
{
    auto const i(this->table_.find(name));

    if (i != this->table_.end())
        return &(*i).second;
    else
        return nullptr;
}
