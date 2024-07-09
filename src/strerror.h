/**
 * @file strerror.h
 *
 * Copyright (C) 2018, 2022  Ossama Othman
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * @author Ossama Othman
 */

#ifndef MARC_STRERROR_H
#define MARC_STRERROR_H

#include <cstring>


namespace MaRC
{
    namespace
    {
        /**
         * @brief Return error string for XSI-compliant case.
         *
         * @param[in] buf Null terminated string containing the error
         *                description.
         *
         * @return Null terminated string containing the error
         *         description.
         *
         * @note Not part of the %MaRC API.
         */
        [[maybe_unused]]
        inline char const * strerror_helper(int /* result */,
                                            char const * buf)
        {
            /**
             * @note We lose the ability to report specific errors in
             *       the underlying C library  @c ::strerror_r() call
             *       itself with this approach, but we're not too
             *       worried about that in this case since we're only
             *       using actual @c errno values, meaning
             *       @c ::strerror_r() should never fail.
             */
            return buf;
        }

        /**
         * @brief Return error string for the GNU case.
         *
         * @param[in] result Null terminated string containing the
         *                   error description.
         *
         * @return Null terminated string containing the error
         *         description.
         *
         * @note Not part of the %MaRC API.
         */
        [[maybe_unused]]
        inline char const * strerror_helper(char const * result,
                                            char const * /* buf */)
        {
            return result;
        }
    }

    /**
     * @brief Get error description for the given error number.
     *
     * Obtain a short description of the error corresponding to the
     * @c errno value @a errnum.  This function exists to work around
     * the case where the GNU C library version of the @c strerror_r()
     * function ends up being used instead of the XSI-compliant
     * version, since both have the same name and parameter types.  In
     * the GNU case, the @a buf argument is not populated, and the
     * error description string is found in the return value.
     * Implement that behavior for XSI-compliant platforms as well.
     *
     * @param[in]     errnum The error number, i.e. an @c errno
     *                       value.
     * @param[in,out] buf    Buffer that may end up containing the
     *                       description corresponding to the
     *                       error number @a errnum.  Whether or
     *                       not this buffer is populated depends
     *                       on the platform.  Do not rely on this
     *                       buffer being populated.  Use the
     *                       return value of this function
     *                       instead.
     * @param[in]     buflen The length of the buffer @a buf
     *                       passed to this function.
     *
     * @return Null terminated string containing the error
     *         description.
     *
     * @note Not part of the %MaRC API.
     */
    inline char const * strerror(int errnum,
                                 char * buf,
                                 std::size_t buflen)
    {
        auto const result = ::strerror_r(errnum, buf, buflen);

        return strerror_helper(result, buf);
    }
}


#endif  // MARC_STRERROR_H


/*
  Local Variables:
  mode: c++
  c-basic-offset: 4
  indent-tabs-mode: nil
  End:
*/
