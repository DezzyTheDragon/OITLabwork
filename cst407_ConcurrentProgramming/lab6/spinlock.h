#pragma once
//*******************************************
// Simple spinlock
//
// Author: Phil Howard

#include "lock.h"
#include <atomic>

class spinlock_t : public lock_t
{
    public:
        spinlock_t() : lock_t() 
        {
            atomic_lock = new std::atomic_flag();
        }

        virtual ~spinlock_t() 
        {
            delete atomic_lock;
        }

        void lock()
        {
            while(atomic_lock->test_and_set())
            {}
        }

        void unlock()
        {
            atomic_lock->clear();
        }
    private:
        std::atomic_flag *atomic_lock;
};
