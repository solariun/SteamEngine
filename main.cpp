#include <iostream>
#include <ctime>
#include "SteamEngine.hpp"
#include <sys/time.h>

using namespace steamengine;

uint32_t steamengine::get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return static_cast<uint32_t>(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void steamengine::sleep(uint32_t timeout)
{
    struct timespec req, rem;
    req.tv_sec = timeout / 1000;
    req.tv_nsec = (timeout % 1000) * 1000000L;

    // Sleep for the requested time
    // If interrupted, sleep for the remaining time
    while (nanosleep(&req, &rem) == -1 && errno == EINTR) {
        req = rem;
    }
}

class thread : public SteamEngineThread
{
public:
    thread() : SteamEngineThread()
    {
        std::cout << "Thread constructor" << std::endl;
        set_nice(1000);
    }
    
    thread(uint32_t nice) : SteamEngineThread()
    {
        std::cout << "Thread constructor, nice:" << nice << std::endl;
        set_nice(nice);
    }

    virtual ~thread() = default;

    virtual thread_return loop()
    {
        std::cout << this << ": Thread running" << std::endl;

        return thread_return::SUCCESS;
    }

    void print()
    {
        std::cout << "Thread print:" << this << std::endl;
    }
};

int main() {
    std::cout << "Hello, Steam Engine!" << std::endl;

    thread t;
    thread t2(100);
    thread t3;
    thread t4(2000);

    // thread t5(100);
    // thread t6(100);
    // thread t7(150);

    for(auto it : t)
    {
        it->print();
    }

    g_default_thread_list.start();
    
    return 0;
}