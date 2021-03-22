#ifndef DIRECTIONALITERATOR_H
#define DIRECTIONALITERATOR_H

#include <iterator>

namespace Types
{
    template <typename T>
    class DirectionalIterator : public std::iterator<std::random_access_iterator_tag, T>
    {
        T* ptr;
        bool direction;

    public:

        DirectionalIterator(T* ptr, bool direction = true) { this->ptr = ptr; this->direction = direction; }
        DirectionalIterator(const DirectionalIterator<T> &other) { this->ptr = other.ptr; this->direction = other.direction; }

        const DirectionalIterator<T> &operator ++() { direction ? ptr++ : ptr--; return *this; }
        const DirectionalIterator<T> &operator --() { direction ? ptr-- : ptr++; return *this; }

        DirectionalIterator<T> operator ++(int) { DirectionalIterator<T> copy(*this); direction ? ptr++ : ptr--; return copy; }
        DirectionalIterator<T> operator --(int) { DirectionalIterator<T> copy(*this); direction ? ptr-- : ptr++; return copy; }

        bool operator ==(const DirectionalIterator<T> &other) const { return ptr == other.ptr; }
        bool operator !=(const DirectionalIterator<T> &other) const { return ptr != other.ptr; }

        bool operator  <(const DirectionalIterator<T>  &other) const { return direction ? ptr < other.ptr : ptr > other.ptr; }
        bool operator  >(const DirectionalIterator<T>  &other) const { return direction ? ptr > other.ptr : ptr < other.ptr; }
        bool operator  <=(const DirectionalIterator<T> &other) const { return direction ? ptr <= other.ptr : ptr >= other.ptr; }
        bool operator  >=(const DirectionalIterator<T> &other) const { return direction ? ptr >= other.ptr : ptr <= other.ptr; }

        DirectionalIterator<T> &operator +=(const long int &add) { direction ? ptr += add : ptr -= add; return *this; }
        DirectionalIterator<T> &operator -=(const long int &sub) { direction ? ptr -= sub : ptr += sub; return *this; }

        DirectionalIterator<T> operator +(const long int &add) const { DirectionalIterator<T> copy(*this); copy += add; return copy; }
        DirectionalIterator<T> operator -(const long int &sub) const { DirectionalIterator<T> copy(*this); copy -= sub; return copy; }

        ptrdiff_t operator -(DirectionalIterator<T> &sub) const { return direction ? ptr - sub.ptr : sub.ptr - ptr; }

        T &operator[](std::size_t idx) { return ptr[direction ? idx : -idx]; }
        T &operator *()  const { return *ptr; }
        T *operator ->() const { return ptr; }

        /**
         * @brief getDirection Get the direction of the iterator
         * @return true for forward, false for backward
         */
        bool getDirection() const { return direction; }

        /**
         * @brief setDirection Set the direction of the iterator
         * @param dir forward, false for backward
         * @return
         */
        void setDirection(bool dir) { direction = dir; }

        /**
         * @brief reverse Reverses the direction of the iterator
         */
        void reverse() { direction = !direction; }
    };
}

#endif // DIRECTIONALITERATOR_H
