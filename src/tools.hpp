/**
 * @file tools.hpp
 * @brief Contains the definition of the StaticList and StaticListContainer classes.
 * 
 * This file provides the implementation of a doubly linked list where objects are automatically
 * added to and removed from the list upon creation and destruction.
 */

#ifndef TOOLS_HPP
#define TOOLS_HPP

// Includes
#include <iostream>

// Namespace
namespace steamengine {

template <typename T>
class Iterator {
public:
    Iterator(T* ptr) : current(ptr) {}

    bool operator!=(const Iterator& other) const {
        return current != other.current;
    }

    T* operator*() const {
        return current;
    }

    const Iterator& operator++() {
        if (current) {
            current = current->next;
        }
        return *this;
    }

private:
    T* current;
};

class StaticListContainer;

// Base class for static list objects
class StaticList {
public:
    StaticList(StaticListContainer& container);
    virtual ~StaticList();

    Iterator<StaticList> begin() const;
    Iterator<StaticList> end() const;

private:
    StaticList* prev;
    StaticList* next;
    StaticListContainer& container;

    friend class Iterator<StaticList>;
};

class StaticListContainer {
public:
    StaticListContainer();

    Iterator<StaticList> begin() const;
    Iterator<StaticList> end() const;

private:
    friend class StaticList;
    StaticList* head;
};

} // namespace steamengine

#endif // TOOLS_HPP
