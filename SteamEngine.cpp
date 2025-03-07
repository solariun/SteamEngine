
#include "SteamEngine.hpp"
#include <sys/time.h>
#include <iostream>

namespace steamengine {

// Local variables
SteamEngine g_default_thread_list;

timeout::timeout() : m_type(timeout_type::INFINITE), m_timeout(0)
{}

timeout::timeout(uint32_t timeout) : m_type(timeout_type::TIMEOUT), m_timeout(timeout)
{
    set_timeout(timeout);
}

void timeout::set_timeout(uint32_t timeout)
{
    m_type = timeout_type::TIMEOUT;
    m_timeout = get_time() + timeout;
}

void timeout::set_infinite()
{
    m_type = timeout_type::INFINITE;
    m_timeout = 0;
}

bool timeout::has_timeout()
{
    return m_type == timeout_type::INFINITE ? false : get_time() >= m_timeout;
}

uint32_t timeout::get_timeout()
{
    return m_timeout;
}

int64_t timeout::get_remaining()
{
    return m_type == timeout_type::INFINITE ? 0 : (uint64_t) m_timeout - (uint64_t) get_time();
}

/* SteamEngineThread */

void SteamEngineThread::default_contruction()
{
    m_thread_list.attach_back(*this);
}

SteamEngineThread::SteamEngineThread(timeout tm) : m_thread_list(g_default_thread_list), m_timeout(tm)
{
    default_contruction();
}

SteamEngineThread::SteamEngineThread(SteamEngine& se, timeout tm) : m_thread_list(se), m_timeout(tm)
{
    default_contruction();
}

SteamEngineThread::~SteamEngineThread()
{
    m_thread_list.detach(*this);
}

bool SteamEngineThread::is_running() const
{
    return m_running;
}

Iterator<SteamEngineThread> SteamEngineThread::begin()
{
    return Iterator<SteamEngineThread>(m_thread_list.begin());
}

Iterator<SteamEngineThread> SteamEngineThread::end()
{
    return Iterator<SteamEngineThread>(m_thread_list.end());
}

void SteamEngineThread::print()
{
    std::cout << "Thread print:" << this << std::endl;
}

thread_state SteamEngineThread::state()
{
    return m_state;
}

thread_return SteamEngineThread::thread_event()
{
    return thread_return::VOID;
}

void  SteamEngineThread::set_nice(uint32_t nice)
{
    m_nice = nice;
}

uint32_t  SteamEngineThread::get_nice()
{
    return m_nice;
}

void SteamEngineThread::set_wait(void* endpoint, uint32_t timeout, uint8_t channel)
{
    m_state = thread_state::WAITING;
    m_timeout.set_timeout(timeout);
    m_endpoint = endpoint;
    m_channel = channel;

    m_attribute = 0;
    m_value = 0;
}

 void SteamEngineThread::set_conditional_wait(void* endpoint, uint32_t timeout, size_t attribute, uint8_t channel)
{
    m_state = thread_state::WAITING;
    m_timeout.set_timeout(timeout);
    m_endpoint = endpoint;
    m_channel = channel;

    m_attribute = attribute;
    m_value = 0;
}

void SteamEngineThread::notify(void* endpoint, size_t attribute, size_t value, uint8_t channel)
{
for(auto it : *this)
    {
        if (it().m_endpoint == endpoint && it().m_channel == channel && (it().m_attribute == 0 || it().m_attribute == attribute))
        {
            it().m_attribute = attribute;
            it().m_value = value;
            it().m_state = thread_state::RUNNING;
        }
    }
}

size_t SteamEngineThread::get_attribute()
{
    return m_attribute;
}

size_t SteamEngineThread::get_value()
{
    return m_value;
}

/* SteamEngine */

LinkItem<SteamEngineThread>* SteamEngine::scheduler()
{
    auto* candidate = current;
    auto thread = (*current)();

    for(size_t nCount=0; nCount < count; nCount++)
    {
        current = (*current)()->get_next();
        current = current != nullptr ? current : first; //to avoid double get_next() call
        thread = (*current)();

        //std::cout << "SteamEngine scheduler, processing:" << thread << ", " << (int) thread->state() << std::endl;

        if (thread->state() == thread_state::STARTING)
        {
            candidate = current;
            goto _exit;
        }
        else if (thread->state() == thread_state::RUNNING || thread->state() == thread_state::WAITING)
        {
            if (thread->m_timeout.get_remaining() < (*candidate)()->m_timeout.get_remaining())
            {
                //std::cout << ">>>> SteamEngine scheduler, Candidate:" << (*candidate)()->m_timeout.get_remaining() << ", Thread:" << thread->m_timeout.get_remaining() << std::endl;
                candidate = current;
            }
        }
        else if (thread->state() == thread_state::STOPPED)
        {
            // Do nothing
        }
    }


    if (candidate != nullptr)
    {
        std::cout << ">>>> SteamEngine scheduler, Candidate:" << candidate << ", " << (*candidate)()->m_timeout.get_remaining() << std::endl;

        auto remaining = (*candidate)()->m_timeout.get_remaining();
        if (remaining > 0)
        {
            sleep(remaining);
        }
    }

_exit:
    return candidate;
}

bool SteamEngine::start()
{
    std::cout << "SteamEngine start" << std::endl;

    if(current != nullptr)
    {
        while(current != nullptr)
        {
            current = scheduler();

            (*current)()->m_state = thread_state::RUNNING;
            (*current)()->loop();
            (*current)()->m_timeout.set_timeout((*current)()->m_nice);
            
            current = current->get_next() != nullptr ? current->get_next() : first;
        }
    }

    std::cout << "SteamEngine end" << std::endl;
    
    return true;
}



} // namespace steamengine