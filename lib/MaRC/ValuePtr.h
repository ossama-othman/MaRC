// -*- C++ -*-

//==========================================================================
/**
 *  @file ValuePtr.h
 *
 *  ValuePtr implementation based on code in Herb Sutter's book "More
 *  Exceptional C++".
 *
 *  @author Ossama Othman
 */
//==========================================================================

#ifndef MARC_VALUE_PTR_H
#define MARC_VALUE_PTR_H

#include <algorithm>

namespace MaRC
{
  /**
   * @struct VP_traits
   *
   * @brief @c ValuePtr traits template structure.
   *
   * The @c ValuePtr template class delegates some operations to this
   * template traits structure.
   */
  template <typename T>
  struct VP_traits
  {
    /// Copy the given object.
    static T * clone (const T * p) { return new T (*p); }
  };

  /**
   * @class ValuePtr
   *
   * @brief Smart pointer implementation designed for use as a class
   *        member.
   *
   * Using a @c std::auto_ptr\<\> as class member is sometimes
   * problematic since ownership of memory is transferred when copying
   * such members.  This @c ValuePtr class is explicitly designed to
   * avoid such problems by performing copies of the underlying object
   * rather than transfer ownership.  This for example, allows it to
   * be readily used as a member in classes placed inside STL
   * containers.
   *
   * @see Item 31 in "More Exceptional C++" by Herb Sutter.
   */
  template <typename T>
  class ValuePtr
  {
  public:

    /// Constructor.
    explicit ValuePtr (T * p = 0) : p_ (p) { }

    /// Destructor.
    ~ValuePtr (void) { delete this->p_; }

    /// Deference operator.
    T & operator* (void) const { return *this->p_; }

    /// Pointer operator.
    T * operator-> (void) const { return this->p_; }

    /// Non-throwing swap operation used to make assignment strongly
    /// exception-safe.
    /**
     * @note As implemented, the swap operation may not work correctly
     *       for @c auto_ptr\<\>s, but why would one use an @c
     *       auto_ptr\<\> as the template argument for this particular
     *       template class!?
     */
    void swap (ValuePtr & other) { std::swap (this->p_, other.p_); }

    /// Copy constructor.
    ValuePtr (const ValuePtr & other)
      : p_ (create_from (other.p_)) { }

    /// Assignment operator.
    ValuePtr & operator= (const ValuePtr & other)
    {
      ValuePtr temp (other);
      this->swap (temp);
      return *this;
    }

    /// Converting copy constructor.
    template <typename U>
    ValuePtr (const ValuePtr<U> & other)
      : p_ (create_from (other.p_)) { }

    /// Converting assignment operator.
    template <typename U>
    ValuePtr & operator= (const ValuePtr<U> & other)
    {
      ValuePtr temp (other);
      this->swap (temp);
      return *this;
    }

  private:

    /// Copying method invoked when copy constructing.
    template <typename U>
    T * create_from (const U * p) const
    {
      return p ? VP_traits<U>::clone (p) : 0;
    }

  private:

    template <typename U> friend class ValuePtr;

    /// Object owned by this @c ValuePtr.
    T * p_;

  };

}

#endif  /* MARC_VALUE_PTR_H */
