// -*- C++ -*-
// Copyright � 1996, 1997, 1998, 2004 Ossama Othman
//
// All Rights Reserved
//
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// $Id: calc.cc,v 1.5 2004/11/08 10:26:24 othman Exp $

#include "calc.h"
#include "parse_scan.h"
#include "parse.h"  /* Parser definiitons */

#include <cmath>


namespace
{
  struct init
  {
    const char * fname;
    double (*fnct)(double);
  };

  struct init arith_fncts[] =
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
  // : type (sym_type)
{
  value.var = 0; /* set value to 0 even if fctn.  */
}

// ---------------------------------------------------------------

MaRC::symrec::symrec (void)
  : table_ ()
{
  for (init * i = arith_fncts; i->fname != 0; ++i)
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
