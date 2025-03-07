/**
 * @file SteamEngine.hpp
 * @brief Contains the definition of the SteamEngine and SteamEngineContainer classes.
 * 
 * This file provides the implementation of a SteamEngine class that extends the StaticList class,
 * allowing it to be automatically added to and removed from a list upon creation and destruction.
 */

#ifndef STEAMENGINE_HPP
#define STEAMENGINE_HPP

// Includes
#include <iostream>
#include "tools.hpp"

// Namespace
namespace steamengine {

class SteamEngineContainer : public StaticListContainer {
public:
    SteamEngineContainer();
};

class SteamEngine : public StaticList {
public:
    SteamEngine(StaticListContainer& container);
    void print() const;
};

} // namespace steamengine

#endif // STEAMENGINE_HPP
