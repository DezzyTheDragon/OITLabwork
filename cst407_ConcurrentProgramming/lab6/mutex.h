#pragma once
//*******************************************
// Wrapper for a c++ mutex
//
// Author: Phil Howard

#include <mutex>
#include "lock.h"

class mutex_t : public lock_t
{
    public:
        mutex_t() : lock_t(), m_lock()
        {}

        virtual ~mutex_t() 
        {}

        void lock()
        {
            m_lock.lock();
        }

        void unlock()
        {
            m_lock.unlock();
        }
    protected:
        std::mutex m_lock;
};
