#pragma once
//*******************************************
// spinlock that implements test-and-test-and set
//
// Author: Phil Howard

#include "lock.h"
#include <atomic>

class spinlock2_t : public lock_t
{
    public:
        spinlock2_t() : lock_t() 
        {
            //atomic_lock = new std::atomic_flag();
            test = 0;
        }

        virtual ~spinlock2_t() 
        {
            //delete atomic_lock;
        }

        void lock()
        {
            bool success = false;
            while(success == 0)
            {
                while(test != 0)
                {}
                int check = 0;
                success = test.compare_exchange_weak(check, 1);
            }
        }

        void unlock()
        {
            //atomic_lock->clear();
            test = 0;
        }
    private:
        //std::atomic_flag *atomic_lock;
        std::atomic<int> test;
};
