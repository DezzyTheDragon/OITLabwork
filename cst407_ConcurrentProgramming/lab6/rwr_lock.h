#pragma once
//*******************************************
// Reader/Writer lock that favors readers.
// Algorithm from The Performance of Spin Lock ALternatives for Shared-Memory
// Multiprocessors by Thomas E. Anderson
//
// Author: Phil Howard

#include "lock.h"
#include <atomic>
#include <stdio.h>

class rwr_lock_t : public lock_t
{
    public:
        rwr_lock_t() : lock_t() 
        {
            atomic_lock = 0;
        }

        virtual ~rwr_lock_t() 
        {}

        void lock()
        {
            write_lock();
        }

        void read_lock()
        {
            atomic_lock += 2;
            while(atomic_lock.load() & 1)
            {}
        }

        void write_lock()
        {
            bool success = false;
            while(success == 0)
            {
                while(atomic_lock != 0) {}
                int check = 0;
                success = atomic_lock.compare_exchange_weak(check, 1);
            }
        }

        void unlock()
        {
            if(atomic_lock.load() & 1)
            {
                //write unlock
                atomic_lock -= 1;
                //printf("Unlocking\n");
            }
            else
            {
                //read unlock
                atomic_lock -= 2;
            }
        }
    private:
        std::atomic<int> atomic_lock;
};
