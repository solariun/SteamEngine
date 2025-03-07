/**
 * @file tools.cpp
 * @brief Implementation of the StaticList and StaticListContainer classes.
 */

#include "tools.hpp"

namespace steamengine {

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
}

// StaticListContainer method implementations
StaticListContainer::StaticListContainer() : head(nullptr) {}

Iterator<StaticList> StaticListContainer::begin() const {
    return Iterator<StaticList>(head);
}

Iterator<StaticList> StaticListContainer::end() const {
    return Iterator<StaticList>(nullptr);
}

} // namespace steamengine
