/**
 * @file SteamEngine.cpp
 * @brief Implementation of the SteamEngine and SteamEngineContainer classes.
 */

#include "SteamEngine.hpp"

namespace steamengine {

// SteamEngineContainer method implementations
SteamEngineContainer::SteamEngineContainer() : StaticListContainer() {}

// SteamEngine method implementations
SteamEngine::SteamEngine(StaticListContainer& container) : StaticList(container) {}

void SteamEngine::print() const {
    std::cout << "Engine ID: " << this << std::endl;
}

} // namespace steamengine
