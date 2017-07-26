// -*- C++ -*-
/**
 * @file calc.h
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

#ifndef MARC_CALC_H
#define MARC_CALC_H

#include <map>
#include <string>   // Standard C++ Library string class


namespace MaRC
{

  struct sym_entry
  {
    sym_entry (void);

    /**
     * @todo This should be private ---- will be rewritten in the
     *       future.
     */
    union
    {
      double var;			/* value of a VAR	*/
      double (*fnctptr)(double);	/* value of a FNCT	*/
    } value;

    int type;		      /* type of symbol: either VAR or FNCT */

  };

  // ---------------------------------------------------------------

  class symrec
  {
  public:

    typedef std::map<std::string, sym_entry> Table;

    symrec (void);

    void putsym (const char *sym_name, int sym_type);

    sym_entry *getsym (const char *sym_name);

  private:

    Table table_;

  };

}


#endif
