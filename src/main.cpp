/**
 * @file main.cpp
 * @brief Demonstrates the usage of the SteamEngine and SteamEngineContainer classes.
 * 
 * This file creates instances of the MyEngine class, which extends the SteamEngine class,
 * and prints their IDs using a for-each loop.
 */

#include "SteamEngine.hpp"

using namespace steamengine;

class MyEngine : public SteamEngine {
public:
    MyEngine(SteamEngineContainer& container) : SteamEngine(container) {}
};

void printEngine(SteamEngine* obj) {
    obj->print();
}

int main() {
    SteamEngineContainer container;

    MyEngine engine1(container);
    MyEngine engine2(container);
    MyEngine engine3(container);

    for (auto obj : container) {
        printEngine(static_cast<SteamEngine*>(obj));
    }

    return 0;
}
