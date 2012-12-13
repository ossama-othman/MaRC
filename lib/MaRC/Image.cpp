// $Id: Image.cpp,v 1.1 2004/07/06 00:01:41 othman Exp $

#include <algorithm>

template <typename T>
MaRC::Image<T>::Image (T * image,
                       unsigned int samples,
                       unsigned int lines)
  : samples_ (image == 0 ? 0 : samples),
    lines_ (image == 0 ? 0 : lines),
    image_ (image)
{
}

template <typename T>
MaRC::Image<T>::Image (unsigned int samples,
                       unsigned int lines)
  : samples_ (samples),
    lines_ (lines),
    image_ (new T [samples * lines])
{
  T * const begin = this->image_;
  T const * const end = this->image_ + samples * lines;

  Image_traits<T>::initialize_image (begin, end);
}

template <typename T>
MaRC::Image<T>::Image (const Image<T> & other)
  : samples_ (other.samples_),
    lines_ (other.lines_),
    image_ (other.samples_ == 0 ? 0 : new T [other.samples_ * other.lines_])
{
  const unsigned int len = other.samples_ * other.lines_;
  for (unsigned int i = 0; i < len; ++i)
    this->image_[i] = other.image_[i];
}

template <typename T>
MaRC::Image<T> &
MaRC::Image<T>::operator= (const Image<T> & other)
{
  Image<T> tmp (other);
  std::swap (this->image_, tmp.image_);

  // Additional non-throwing operations to be performed *after* tmp
  // construction.  This is necessary to ensure strong
  // exception-safety since the tmp copy constructor can throw.
  this->samples_ = other.samples_;
  this->lines_   = other.lines_;

  return *this;
}

template <typename T>
MaRC::Image<T>::~Image (void)
{
  delete [] this->image_;
}

template <typename T>
bool
MaRC::Image<T>::operator== (const Image<T> & rhs) const
{
  if (this != &rhs)
    {
      if (this->samples_ == rhs.samples_
          && this->lines_ == rhs.lines_)
        {
          const unsigned int len = this->samples_ * this->lines_;
          for (unsigned int i = 0; i < len; ++i)
            if (this->image_[i] != rhs.image_[i])
              return false;
        }
      else
        return false;
    }

  return true;
}
