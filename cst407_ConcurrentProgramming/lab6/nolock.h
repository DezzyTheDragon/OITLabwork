#pragma once
//*******************************************
// simple spin-lock 

#include <mutex>
#include "lock.h"

class nolock_t : public lock_t
{
    public:
        nolock_t() 
        {}

        virtual ~nolock_t() 
        {}

        void lock()
        {
            // noop
        }

        void unlock()
        {
            // noop
        }
};
