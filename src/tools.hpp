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
    StaticListContainer() : head(nullptr) {}

    Iterator<StaticList> begin() const {
        return Iterator<StaticList>(head);
    }

    Iterator<StaticList> end() const {
        return Iterator<StaticList>(nullptr);
    }

private:
    friend class StaticList;
    StaticList* head;
};

// StaticList method implementations
StaticList::StaticList(StaticListContainer& container) : prev(nullptr), next(nullptr), container(container) {
    std::cout << "Adding object to list" << std::endl;
    if (container.head) {
        container.head->prev = this;
        next = container.head;
    }
    container.head = this;
}

StaticList::~StaticList() {
    std::cout << "Deleting object from list" << std::endl;
    if (prev) {
        prev->next = next;
    } else {
        container.head = next;
    }
    if (next) {
        next->prev = prev;
    }
}

Iterator<StaticList> StaticList::begin() const {
    return Iterator<StaticList>(container.head);
}

Iterator<StaticList> StaticList::end() const {
    return Iterator<StaticList>(nullptr);
};

} // namespace steamengine

#endif // TOOLS_HPP
