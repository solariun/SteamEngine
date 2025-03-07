#ifndef STEAMENGINE_HPP
#define STEAMENGINE_HPP

// Includes
#include <iostream>
#include "tools.hpp"

// Namespace
namespace steamengine {

class SteamEngineContainer : public StaticListContainer {
public:
    SteamEngineContainer() : StaticListContainer() {}
};

class SteamEngine : public StaticList {
public:
    SteamEngine(StaticListContainer& container) : StaticList(container) {}
    void print() const {
        std::cout << "Engine ID: " << this << std::endl;
    }
};

} // namespace steamengine

#endif // STEAMENGINE_HPP
