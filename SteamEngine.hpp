/**
 * @file SteamEngine.hpp
 * @brief Header file for the SteamEngine project, which provides a lightweight contextless threading mechanism in C++.
 *
 * This file contains the declaration of the SteamEngine class, which is responsible for managing a lightweight thread.
 */

/**
 * @class SteamEngine
 * @brief A class that provides a lightweight contextless threading mechanism.
 *
 * The SteamEngine class allows for starting, stopping, and checking the status of a thread.
 */
#ifndef STEAMENGINE_HPP
#define STEAMENGINE_HPP

#include <iostream>

namespace steamengine {

uint32_t get_time();
void sleep(uint32_t timeout);

/**
 * @brief General purpose iterator facility
 * 
 * @tparam T 
 */
template <typename T> class Iterator
{
public:
    Iterator() = delete;

    /**
     * @brief Type based constructor
     *
     * @param ptr Type pointer to iterate
     */
    Iterator(T*  ptr) : m_ptr(ptr)
    {}

    /*
    * Access operator
    */
    Iterator<T> operator*()
    {
        return *this;
    }
    
    T& operator()()
    {
        return *m_ptr;
    }

    T* operator->()
    {
        return m_ptr;
    }

    /*
    * Movement operator
    */
    Iterator<T>& operator++()
    {
        if (m_ptr != nullptr)
        {
            m_ptr = m_ptr->operator++ ();
        }
        
        return *this;
    }

    /*
    * Binary operators
    */
    friend bool operator== (const Iterator& a, const Iterator& b){ return a.m_ptr == b.m_ptr;};
    friend bool operator!= (const Iterator& a, const Iterator& b){ return a.m_ptr != b.m_ptr;};

private:
    T* m_ptr = nullptr;
};

/**
 * @brief Link Item to turn any object attachable to LinkList
 * 
 * @tparam T    Type to be managed
 */
template<class T>class LinkItem
{
public:

    /**
     * @brief Easy way to have access to the item object
     * 
     * @return T&   Referece to the item object
     */
    T* operator()()
    {
        return (T*)this;
    }

    T* operator++ (void)
    {
        return (T*)next;
    }

    LinkItem<T>* get_next (void)
    {
        return next;
    }

private:
    template<class U> friend class List;
    template <typename U> friend class iterator;

    LinkItem<T>* next = nullptr;
    LinkItem<T>* prev = nullptr;
};

/**
 * @brief Doubly linked list for managing LinkItem objects
 * 
 * @tparam T    Type of the object
 * 
 * @note    Note that this does not create memory, it uses next/prev added to the object 
 *          by extending the object to LinkItem and manages its life cycle, is automatically
 *          inserted and deleted on instantiation and object destruction.
 */
template<class T> class List
{
public:

    /**
     * @brief Attach a LinkItem enabled object to the list
     * 
     * @param list_item  The object
     * 
     * @return true if it was successfully attached
     */
    bool attach_back(LinkItem<T>& list_item)
    {
        if (first == nullptr)
        {
            first = &list_item;
            last = first;
            current = first;
        }
        else
        {
            list_item.prev = last;
            last->next = &list_item;
            last = &list_item;
        }

        count++;

        return true;
    }

    /**
     * @brief Detach a specific LinkItem from the managed list
     * 
     * @param list_item  LinkItem enabled object to be deleted
     * 
     * @return true     if successfully detached
     */
    bool detach(LinkItem<T>& list_item)
    {
        if (count == 0)
        {
            return false;
        }

        if (list_item.next == nullptr && list_item.prev == nullptr)
        {
            first = nullptr;
            list_item.prev = nullptr;
            current = nullptr;
        }
        else if (list_item.prev == nullptr)
        {
            list_item.next->prev = nullptr;
            first = list_item.next;
            current = first;
        }
        else if (list_item.next == nullptr)
        {
            list_item.prev->next = nullptr;
            last = list_item.prev;
            current = last;
        }
        else
        {
            list_item.prev->next = list_item.next;
            list_item.next->prev = list_item.prev;
            current = list_item.next->prev;
        }

        count--;

        return true;
    }

    size_t size()
    {
        return count;
    }
        
    /**
     * @brief iterator helper for signaling beginning
     * 
     * @return iterator<LinkItem<T>>  the first item
     */
    T* begin()
    {
        return (T*)first;
    }

    /**
     * @brief iterator helper for signaling ending
     * 
     * @return iterator<LinkItem<T>>  the end of the list
     */
    /**
     * @brief Returns an iterator to the end (one past the last element) of the container.
     * 
     * This function provides an iterator that points to the position just after the last element
     * in the container. It is typically used in conjunction with the begin() function to iterate
     * over the elements of the container.
     * 
     * @return Iterator<LinkItem<T>> An iterator to the end of the container.
     */
    T* end()
    {
        return (T*)nullptr;
    }

protected:
    template <typename U> friend class iterator;

    LinkItem<T>* current = nullptr;
    LinkItem<T>* first = nullptr;
    LinkItem<T>* last = nullptr;

    size_t count = 0;
};

class timeout
{
public:
    enum class timeout_type
    {
        TIMEOUT,
        INFINITE,
    };

    timeout();
    timeout(uint32_t timeout);

    void set_timeout(uint32_t timeout);
    void set_infinite();
    bool has_timeout();
    uint32_t get_timeout();
    int64_t get_remaining();
    
private:
    timeout_type m_type;
    uint32_t m_timeout;
};

class SteamEngineThread;

class SteamEngine : public List<SteamEngineThread>
{
public:
    SteamEngine() = default;
    virtual ~SteamEngine() = default;

    LinkItem<SteamEngineThread>* scheduler();
    bool start();
};

enum class thread_state {
    STARTING,
    RUNNING,
    STOPPED,
    WAITING,
};

enum class thread_return {
    SUCCESS,   // Used for general success
    FAILURE,   // Used for general failure
    VOID,      // Used for void functions
};

extern SteamEngine g_default_thread_list;

class SteamEngineThread : public LinkItem<SteamEngineThread>
{
public:
    SteamEngineThread(timeout tm = timeout(0));
    SteamEngineThread(SteamEngine& se, timeout tm = timeout(0));
    virtual ~SteamEngineThread();

    bool is_running() const;

    Iterator<SteamEngineThread> begin();
    Iterator<SteamEngineThread> end();

    void set_wait(void* endpoint, uint32_t timeout, uint8_t channel=0);
    void set_conditional_wait(void* endpoint, uint32_t timeout, size_t attribute=0, uint8_t channel=0);
    void notify(void* endpoint, size_t attribute, size_t value, uint8_t channel=0);

    size_t get_attribute();
    size_t get_value();

    void print();

    thread_state state();

protected:
    virtual thread_return loop() = 0;
    virtual thread_return thread_event();

    void set_nice(uint32_t nice);
    uint32_t get_nice();
    
private:
    void default_contruction();

    friend class SteamEngine;

    SteamEngine &m_thread_list;
    bool m_running;
    timeout m_timeout{};
    uint32_t m_nice{0};
    thread_state m_state{thread_state::STARTING};

    // Waiting condition and data
    void* m_endpoint{nullptr};
    size_t m_attribute{0};
    size_t m_value{0};
    uint8_t m_channel{0};
};

} // namespace steamengine
#endif // STEAMENGINE_HPP