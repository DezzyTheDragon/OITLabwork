#pragma once
//************************************************
// Timer class
// ***********************************************
#include <chrono>
using namespace std::chrono;

class timer
{
    public:
        timer() : 
            m_start(steady_clock::now()),
            m_stop(steady_clock::now())
        {}

        void start()
        {
            m_start = steady_clock::now();
        }

        void stop()
        {
            m_stop = steady_clock::now();
        }

        double duration()
        {
            stop();
            std::chrono::duration<double> dur = 
                std::chrono::duration_cast<std::chrono::duration<double>>(m_stop - m_start);
            return dur.count();
        }

        void delay(double time)
        {
            start();
            while (duration() < time)
            {}
        } 
    protected:
        steady_clock::time_point m_start;
        steady_clock::time_point m_stop;

};
