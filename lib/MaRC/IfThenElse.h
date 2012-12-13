// -*- C++ -*-

//=============================================================================
/**
 *  @file    IfThenElse.h
 *
 *  @c MaRC::IfThenElse traits template based on the @c IfThenElse
 *  template described in the book "C++ Templates" by Vandevoorde and
 *  Josuttis.
 *
 *  $Id: IfThenElse.h,v 1.1 2004/10/11 06:20:53 othman Exp $
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef MARC_IF_THEN_ELSE_H
#define MARC_IF_THEN_ELSE_H

namespace MaRC
{

  /**
   * @struct IfThenElse
   *
   * @brief Compile-time selection of type based on a boolean value.
   *
   * This primary template selects the second or third argument based
   * on the value of the boolean first argument.
   *
   * Usage example:
   *
   * \code
   *
   *   template <typename T>
   *   class Foo
   *   {
   *   public:
   *     // Set "TheType" to be the larger of "T" and "int".
   *     typedef typename IfThenElse<(sizeof (T) > sizeof (int)),
   *                                 T,
   *                                 int>::result_type TheType;
   *   };
   *
   * \endcode
   *
   * @note This merely a forward declaration since we really only care
   *       about the partial specializations below.
   */
  template <bool C, typename Ta, typename Tb>
  struct IfThenElse;

  /**
   * @struct IfThenElse
   *
   * @brief Select of type @a Ta if boolean value is @c true.
   *
   * This partial specialization selects the type @a Ta if the boolean
   * first argument is @c true.
   */
  template <typename Ta, typename Tb>
  struct IfThenElse<true, Ta, Tb>
  {
    typedef Ta result_type;
  };

  /**
   * @struct IfThenElse
   *
   * @brief Select of type @a Tb if boolean value is @c false.
   *
   * This partial specialization selects the type @a Tb if the boolean
   * first argument is @c false.
   */
  template <typename Ta, typename Tb>
  struct IfThenElse<false, Ta, Tb>
  {
    typedef Tb result_type;
  };

}

#endif  /* MARC_IF_THEN_ELSE_H */
