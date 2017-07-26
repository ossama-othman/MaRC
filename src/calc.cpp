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


namespace
{
  struct init
  {
    const char * fname;
    double (*fnct)(double);
  };

  init const arith_fncts[] =
    {
      { "sin", sin },
      { "cos", cos },
      { "tan", tan },
      { "asin", asin },
      { "acos", acos },
      { "atan", atan },
      { "ln", log },
      { "exp", exp },
      { "sqrt", sqrt },
      { 0, 0 }
    };
}

// ---------------------------------------------------------------

MaRC::sym_entry::sym_entry (void)
   : type()
{
  value.var = 0; /* set value to 0 even if fctn.  */
}

// ---------------------------------------------------------------

MaRC::symrec::symrec (void)
  : table_ ()
{
  for (init const * i = arith_fncts; i->fname != 0; ++i)
    {
      sym_entry s;
      s.type = FNCT;
      s.value.fnctptr = i->fnct;

      this->table_[i->fname] = s;
    }
}

void
MaRC::symrec::putsym (char const * name, int type)
{
  sym_entry s;
  s.type = type;

  Table::value_type const d (name, s);

  // Use insert() instead of operator[] to prevent users from
  // overwriting function entries we added.
  (void) this->table_.insert (d);
}

MaRC::sym_entry *
MaRC::symrec::getsym (char const * sym_name)
{
  Table::iterator const i (this->table_.find (sym_name));

  if (i != this->table_.end ())
    return &(*i).second;
  else
    return 0;
}
