#ifndef TREE_H
#define TREE_H

#include "directionaliterator.h"

#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

namespace Types
{
    template <typename T, typename U = std::string>
    class Tree
    {
        Tree<T, U> *parent = nullptr;

        U label;
        bool labeled = false;

        Tree<T, U> *left_node  = nullptr;
        Tree<T, U> *right_node = nullptr;

        std::map<U, Tree<T, U>*> children;

        Tree<T, U> *first_child  = nullptr;
        Tree<T, U> *last_child   = nullptr;

        T contents;

        Tree(const T &t, Tree<T, U> *parent) { this->contents = t; this->parent = parent;  }
        Tree(T &&t, Tree<T, U> *parent)      { this->contents = std::move(t); this->parent = parent; }

        Tree(const T &t, Tree<T, U> *parent, U label) : Tree(t, parent)            { this->label = label; labeled = true;  }
        Tree(T &&t, Tree<T, U> *parent, U label)      : Tree(std::move(t), parent) { this->label = label; labeled = true; }

        void appendChild(Tree<T, U> *child)
        {
            if(first_child)
            {
                last_child->right_node = child;
                child->left_node = this->last_child;
            }
            else
            {
                first_child = child;
            }

            last_child = child;
        }

     public:

        /**
         * @brief Tree Creates a tree node containing t
         * @param t
         */
        Tree(const T &t) : Tree(t, nullptr) { }

        /**
         * @brief Tree Creates a tree node containing t using move semantics
         * @param t
         */
        Tree(T &&t) : Tree(std::move(t), nullptr) {  }

        /**
         * @brief Tree Copy constructor
         * @param other
         */
        Tree(const Tree<T, U> &other) { contents = other.contents; *this = other; }

        /**
         * @brief Tree Move constructor
         * @param other
         */
        Tree(Tree<T, U> &&other) { contents = other.contents; *this = std::move(other); }

        ~Tree()
        {
            if(labeled && parent)
                parent->children[label] = nullptr;

            if(left_node)  left_node->right_node = right_node;
            if(right_node) right_node->left_node = left_node;

            clear();
        }

        /**
         * @brief operator = Copy operator
         * @param other
         * @return
         */
        Tree<T, U> &operator=(const Tree<T, U> &other)
        {
            contents = other.contents;
            label = other.label;

            children.clear();
            for(Tree<T, U> &a : *this)
                delete &a;

            for(Tree<T, U> &c : other)
            {
                Tree<T, U> *cc = new Tree<T, U>(c);
                appendChild(cc);
                if(c.labeled)
                    children[c.label] = cc;
            }

            return *this;
        }

        /**
         * @brief operator = Move operator
         * @param other
         * @return
         */
        Tree<T, U> &operator=(Tree<T, U> &&other)
        {
            contents = std::move(other.contents);

            children.clear();
            for(auto a : *this)
                delete &*a;

            first_child = nullptr;
            last_child  = nullptr;

            for(Tree<T, U> &c : other)
            {
                appendChild(&c);
                if(c.labeled)
                    children[c.label] = &c;
            }

            other.first_child = nullptr;
            other.last_child  = nullptr;

            return *this;
        }

        /**
         * @brief operator ==
         * @param other
         * @return true if both trees contain the same elements in the same order (compared with !=)
         */
        bool operator ==(const Tree<T, U> &other) const
        {
            if(contents != other.contents)
                return false;

            auto it = other.begin();
            for(Tree<T, U> &c : *this)
            {
                if(it.getPtr() == nullptr)
                    return false;

                if(c != *it)
                    return false;

                it++;
            }

            if(it.getPtr() != nullptr)
                return false;

            return true;
        }

        /**
         * @brief operator !=
         * @param other
         * @return true if both trees contain the same elements in the same order (compared with ==)
         */
        bool operator !=(const Tree<T, U> &other) const
        {
            if(contents != other.contents)
                return true;

            auto it = other.begin();
            for(Tree<T, U> &c : *this)
            {
                if(it.getPtr() == nullptr)
                    return true;

                if(c != *it)
                    return true;

                it++;
            }

            if(it.getPtr() != nullptr)
                return true;

            return false;
        }

        /**
         * @brief operator [] Access child
         * @param idx Index of child
         * @return
         */
        Tree<T, U>* operator[](U label) { return children[label]; }

        /**
         * @brief getContents Access contained data
         * @return Reference to contained data
         */
        T &getContents() { return contents; }

        Tree<T, U> *getParent() { return parent; }

        /**
         * @brief operator * same as getContents()
         * @return
         */
        T &operator*() { return getContents(); }


        void addChild(T &t)           { appendChild(new Tree<T, U>(t,            this)); }
        void addChild(T &&t)          { appendChild(new Tree<T, U>(std::move(t), this)); }

        void setChild(T &t,  U label)
        {
            Tree<T, U> *child = new Tree<T, U>(t, this, label);
            if(children[label] == nullptr) appendChild(child);
            children[label] = child;
        }

        void setChild(T &&t,  U label)
        {
            Tree<T, U> *child = new Tree<T, U>(std::move(t), this, label);
            if(children[label] == nullptr) appendChild(child);
            children[label] = child;
        }

        U getLabel()
        {
            return label;
        }

        class TreeIterator : public std::iterator<std::bidirectional_iterator_tag, T>
        {
        public:
            enum WENT_OVER { OVER_LEFT, OVER_NOT, OVER_RIGHT };
        private:

            Tree<T, U>* ptr;
            Tree<T, U>* prePtr;

            WENT_OVER over;

        public:

            TreeIterator(Tree<T, U>* ptr, Tree<T, U>* pp = nullptr, WENT_OVER o = OVER_NOT) { this->ptr = ptr;       this->prePtr = pp;           this->over = o;          }
            TreeIterator(const TreeIterator &other)                                         { this->ptr = other.ptr; this->prePtr = other.prePtr; this->over = other.over; }

            const TreeIterator &operator ++()
            {
                if(over == OVER_LEFT)  { ptr    = prePtr; }
                if(over == OVER_NOT )  { prePtr = ptr;    ptr = ptr->right_node; }
                over = ptr == nullptr ? OVER_RIGHT : OVER_NOT;
                return *this;
            }
            const TreeIterator &operator --()
            {
                if(over == OVER_RIGHT) { ptr    = prePtr; }
                if(over == OVER_NOT )  { prePtr = ptr;    ptr = ptr->left_node; }
                over = ptr == nullptr ? OVER_LEFT : OVER_NOT;
                return *this;
            }

            TreeIterator operator ++(int) { TreeIterator copy(*this); ++*this; return copy; }
            TreeIterator operator --(int) { TreeIterator copy(*this); --*this; return copy; }

            bool operator ==(const TreeIterator &other) const { return ptr == other.ptr; }
            bool operator !=(const TreeIterator &other) const { return ptr != other.ptr; }

            Tree<T, U>  &operator *()  const { return *ptr; }
            Tree<T, U>  *operator ->() const { return ptr; }

            Tree<T, U> *getPtr() { return ptr; };
        };

        TreeIterator begin()  const { return TreeIterator(first_child); }
        TreeIterator end()    const { return TreeIterator(nullptr, last_child, TreeIterator::OVER_RIGHT); }

        TreeIterator rbegin() const { return TreeIterator(last_child); }
        TreeIterator rend()   const { return TreeIterator(nullptr, first_child, TreeIterator::OVER_LEFT); }

        /**
         * @brief clear Deletes all children
         */
        void clear()
        {
            std::vector<Tree<T, U>*> del_ptrs;

            for(Tree<T, U> &a : *this)
                del_ptrs.push_back(&a);

            for(Tree<T, U>* ptr : del_ptrs)
                delete ptr;

            first_child = nullptr;
            last_child  = nullptr;
        }
    };
}

#endif // TREE_H
