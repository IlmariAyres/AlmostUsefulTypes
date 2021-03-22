#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <utility>

#include "directionaliterator.h"

namespace Types
{
    template <typename T>
    class Stack
    {
        size_t size_real = 0;

        T *real_begin = 0;
        T *data_begin, *data_end;

        /**
         * @brief direction Direction of the stack; true for forward, false for backward
         */
        bool direction = true;

        /**
         * @brief push_back_i Pushes val to to place behind data_end, resizes if needed
         * @param val
         */
        void push_back_i(const T &val)
        {
            if(data_end >= real_begin + size_real)
                resize(size_real * 2);

            *data_end++ = val;
        }

        /**
         * @brief push_back_i Pushes val to to place preceding data_begin, resizes if needed
         * @param val
         */
        void push_front_i(const T &val)
        {
            if(data_begin <= real_begin)
                resize(size_real * 2);

            *(--data_begin) = val;
        }

        /**
         * @brief push_back_i Pushes val to to place behind data_end, resizes if needed
         * @param val
         */
        void pop_back_i() { (--data_end)->~T(); }

        /**
         * @brief push_back_i Pushes val to to place preceding data_begin, resizes if needed
         * @param val
         */
        void pop_front_i() { data_begin->~T(); data_begin++; }

        /**
         * @brief init
         * @param size Optional argument to set the initial amount of elements to hold
         * @param buffer Optional argument to set size of empy area around the stack to facilitate new members
         */
        void init(size_t size = 8, size_t buffer = 0)
        {
            size_real = size + 2 * buffer;
            real_begin = new T[size + 2 * buffer];
            data_begin = data_end = real_begin + buffer;
        }

        /**
         * @brief resize Resizes the stack to a new size to fit more elements
         * @param new_size
         */
        void resize(size_t new_size)
        {
            T* o_real_begin = real_begin;
            T* o_data_begin = data_begin;
            size_t o_size = size();

            init(new_size, new_size / 2);

            for(size_t i = 0; i < o_size; i++)
                data_begin[i] = std::move(o_data_begin[i]);
            data_end += o_size;

            delete o_real_begin;
        }

    public:
        /**
         * @brief Stack
         * @param size Optional argument to set the initial amount of elements to hold
         * @param buffer Optional argument to set size of empy area around the stack to facilitate new members
         */
        Stack(size_t size = 8, size_t buffer = 0)
        {
            init(size, buffer);
        }

        /**
         * @brief Stack Copy constructor
         * @param other
         */
        Stack(const Stack<T> &other) { *this = other; }

        /**
         * @brief Stack Move constructor
         * @param other
         */
        Stack(Stack<T> &&other) { *this = std::move(other); }

        /**
         * @brief Stack Constructor to populate stack with contents provided list
         * @param list
         * @param direction Direction of the stack; true for forward, false for backward
         */
        Stack(std::initializer_list<T> list, bool direction = true)
        {
            init(list.size(), list.size() / 2);

            for(int i = 0; i < list.size(); i++)
                data_begin[i] = list.begin()[i];

            data_end += list.size();

            this->direction = direction;
        }

        ~Stack()
        {
            if(real_begin)
            {
                for(T &e : *this)
                    e.~T();
                delete[] real_begin;
            }
            real_begin = data_begin = data_end = nullptr;
        }

        /**
         * @brief operator = Copy operator
         * @param other
         * @return
         */
        Stack<T> &operator=(const Stack<T> &other)
        {
            this->~Stack<T>();

            init(other.size(), other.size() / 2);

            for(size_t i = 0; i < other.size(); i++)
                data_begin[i] = std::move(other.rbegin()[i]);
            data_end += other.size();

            return *this;
        }

        /**
         * @brief operator = Move operator
         * @param other
         * @return
         */
        Stack<T> &operator=(Stack<T> &&other)
        {
            real_begin = other.real_begin;
            data_begin = other.data_begin;
            data_end   = other.data_end;
            size_real  = other.size_real;
            direction  = other.direction;

            other.real_begin = other.data_begin = other.data_end = nullptr;
            other.~Stack<T>();

            return *this;
        }

        /**
         * @brief operator ==
         * @param other
         * @return true if both stacks contain the same elements in the same order (compared with !=)
         */
        bool operator ==(const Stack<T> &other) const
        {
            if(other.size() != size())
                return false;

            for(int i = 0; i < size(); i++)
                if((*this)[i] != other[i])
                    return false;

            return true;
        }

        /**
         * @brief operator !=
         * @param other
         * @return false if both stacks contain the same elements in the same order (compared with ==)
         */
        bool operator !=(const Stack<T> &other) const
        {
            if(other.size() != size())
                return true;

            for(int i = 0; i < size(); i++)
                if((*this)[i] != other[i])
                    return true;

            return false;
        }

        /**
         * @brief operator += Pushes add on top of the stack
         * @param add
         * @return
         */
        Stack<T> &operator +=(const Stack<T> &add)
        {
            for(DirectionalIterator<T> i = add.rbegin(); i != add.rend(); i++)
                push_top(*i);

            return *this;
        }

        Stack<T> operator +(const Stack<T> &add) const { Stack<T> copy = *this; copy += add; return copy; }


        /**
         * @brief operator []
         * @param idx Index of item
         * @return
         */
        T &operator[](std::size_t idx) const
        {
            if(idx >= size())
                throw "Stack index out of bounds";

            return this->begin()[idx];
        }

        /**
         * @brief size
         * @return Number of elements stored
         */
        size_t size() const
        {
            return data_end - data_begin;
        }

        /**
         * @brief clear Clears the stack
         */
        void clear()
        {
            this->~Stack<T>();
            init();
        }

        DirectionalIterator<T> begin()    const { return DirectionalIterator<T>(direction ? data_end   - 1 : data_begin    , !direction); }
        DirectionalIterator<T> rbegin()   const { return DirectionalIterator<T>(direction ? data_begin     : data_end   - 1,  direction); }
        DirectionalIterator<T> end()      const { return DirectionalIterator<T>(direction ? data_begin - 1 : data_end      , !direction); }
        DirectionalIterator<T> rend()     const { return DirectionalIterator<T>(direction ? data_end       : data_begin - 1,  direction); }

        /**
         * @brief pull_top Returns item at the top of the stack and deletes it
         * @return
         */
        T pull_top()
        {
            if(size())
            {
                T t = std::move(*(this->begin()));
                pop_top();
                return std::move(t);
            }
            else return 0;
        }

        /**
         * @brief pull_bottom Returns item at the top of the stack and deletes it
         * @return
         */
        T pull_bottom()
        {
            if(size())
            {
                T t = std::move(*(this->rbegin()));
                pop_bottom();
                return std::move(t);
            }
            else return 0;
        }

        /**
         * @brief push_top Pushes val to the top of the stack
         * @param val
         */
        void push_top(const T &val) { direction ? push_back_i(val) : push_front_i(val); };

        /**
         * @brief push_top Pushes val to the bottom of the stack
         * @param val
         */
        void push_bottom(const T &val) { direction ? push_front_i(val) : push_back_i(val); };

        /**
         * @brief pop_top Deletes item at the top of the stack
         */
        void pop_top()    { if(size()) direction ? pop_back_i() : pop_front_i(); }

        /**
         * @brief pop_bottom Deletes item at the bottom of the stack
         */
        void pop_bottom() { if(size()) direction ? pop_front_i() : pop_back_i();}

        /**
         * @brief getDirection Get the direction of the stack
         * @return true for forward, false for backward
         */
        bool getDirection() const { return direction; }

        /**
         * @brief setDirection Set the direction of the stack
         * @param dir forward, false for backward
         * @return
         */
        void setDirection(bool dir) { direction = dir; }

        /**
         * @brief reverse Reverses the direction of the stack
         */
        void reverse() { direction = !direction; }
    };
}

#endif // STACK_H
