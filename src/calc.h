/**
 * @file calc.h
 *
 * Copyright (C) 1996-1998, 2004, 2017, 2020  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_CALC_H
#define MARC_CALC_H

#include <map>
#include <string>


namespace MaRC
{

    /**
     * @struct sym_entry
     *
     * @brief Calculator symbol table value.
     */
    struct sym_entry
    {
        using function_type = double (*)(double);

        // Disable default construction.
        sym_entry() = delete;

        /// Construct a function @c sym_entry.
        explicit sym_entry(function_type fnct);

        /// Construct a variable @c sym_entry.
        explicit sym_entry(double var);

        /**
         * @brief The underlying symbol table value, either a variable
         *        or a function.
         *
         * @todo This should be private.
         */
        union sym_value
        {
            explicit sym_value(double v) : var(v) {}
            explicit sym_value(function_type f) : fnctptr(f) {}

            /// Value of a @c VAR.
            double var;

            /// Value of a @c FNCT.
            function_type fnctptr;
        };

        /// Symbol value.
        sym_value value;

        /// Type of symbol: either VAR or FNCT.
        int type;
    };

    // ---------------------------------------------------------------

    /**
     * @struct symrec
     *
     * @brief Calculator symbol table.
     */
    class symrec
    {
    public:

        /// Underlying calculator symbol map.
        using table_type = std::map<std::string, sym_entry>;

        /// Constructor
        symrec();

        symrec(symrec const &) = delete;
        void operator=(symrec const &) = delete;

        /**
         * @brief Add symbol to the table.
         *
         * @param[in] name The symbol name.
         * @param[in] type The symbol type, either @c VAR or @c FNCT.
         */
        void putsym(char const * name, int type);

        /**
         * @brief Get symbol from the table.
         *
         * @param[in] name The symbol name.
         *
         * @return Pointer to the symbol table entry corresponding to
         *         symbol @a name.
         */
        sym_entry * getsym(char const * name);

    private:

        /// The underlying symbol map.
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
