//****************************************************
// Definitions for functions to access a Thread Safe Counter
// Author: Phil Howard
//
#include <pthread.h>

#include "tscounter.h"

static pthread_mutex_t g_ts_lock = PTHREAD_MUTEX_INITIALIZER;

static long g_ts_counter = 0;

// ***************************************************
// Reset the thread safe counter to zero
void tsc_reset(long value)
{
    pthread_mutex_lock(&g_ts_lock);
    g_ts_counter = value;
    pthread_mutex_unlock(&g_ts_lock);
}

// ***************************************************
// Increment the thread safe counter. 
// Return the value after the increment.
long  tsc_increment()
{
    long value;

    pthread_mutex_lock(&g_ts_lock);
    value = ++g_ts_counter;
    pthread_mutex_unlock(&g_ts_lock);

    return value;
}

// ***************************************************
// Decrement the thread safe counter.
// Return the value after the decrement.
long  tsc_decrement()
{
    long value;

    pthread_mutex_lock(&g_ts_lock);
    value = --g_ts_counter;
    pthread_mutex_unlock(&g_ts_lock);

    return value;
}

// ***************************************************
// return the value of the thread safe counter
long  tsc_value()
{
    long value;

    pthread_mutex_lock(&g_ts_lock);
    value = g_ts_counter;
    pthread_mutex_unlock(&g_ts_lock);

    return value;
}

