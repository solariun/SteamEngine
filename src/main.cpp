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
