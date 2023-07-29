#pragma once
//*******************************************
// lock base class

class lock_t
{
    public:
        lock_t() {}
        virtual ~lock_t() {}

        virtual void lock() = 0;
        virtual void read_lock()  { lock(); }
        virtual void write_lock() { lock(); }
        virtual void unlock() = 0;
};
