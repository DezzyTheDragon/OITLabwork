#pragma once
//*******************************************
// Reader/Writer lock that favors writers.
// Algorithm from The Performance of Spin Lock ALternatives for Shared-Memory
// Multiprocessors by Thomas E. Anderson
//
// Author: Phil Howard

#include "lock.h"
#include <atomic>

class rww_lock_t : public lock_t
{
    public:
        rww_lock_t() : lock_t() 
        {
            read_count = 0;
            write_req = 0;
            write_comp = 0;
        }

        virtual ~rww_lock_t() 
        {}

        void lock()
        {
            write_lock();
        }

        void read_lock()
        {
            while(write_req != write_comp);
            read_count += 2;
            while(read_count.load() & 1);
        }

        void write_lock()
        {
            int prev = write_req++;
            while(prev != write_comp.load());
            bool success = false;
            while(success == 0)
            {
                int check = 0;
                success = read_count.compare_exchange_weak(check, 1);
            }
        }

        void unlock()
        {
            if(read_count.load() & 1)
            {
                read_count -= 1;
                write_comp++;
            }
            else
            {
                read_count -= 2;
            }
        }
    private:
        std::atomic<int> read_count;
        std::atomic<int> write_req;
        std::atomic<int> write_comp;
};
