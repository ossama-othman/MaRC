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
        sym_entry() = delete;

        /// Construct a function @c sym_entry.
        explicit sym_entry(double (*fnctptr)(double));

        /// Construct a variable @c sym_entry.
        explicit sym_entry(double var);

        /**
         * @todo This should be private
         */
        union
        {
            /// Value of a VAR.
            double var;

            /// Value of a FNCT.
            double (*fnctptr)(double);
        } value;

        /// Type of symbol: either VAR or FNCT.
        int type;
    };

    // ---------------------------------------------------------------

    class symrec
    {
    public:

        typedef std::map<std::string, sym_entry> table_type;

        symrec();

        void putsym(char const * sym_name, int sym_type);

        sym_entry * getsym(char const * sym_name);

    private:

        table_type table_;

    };

}


#endif


/*
  Local Variables:
  mode: c++
  c-basic-offset: 4
  indent-tabs-mode: nil
  End:
*/
