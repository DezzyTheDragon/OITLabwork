#pragma once
//****************************************************
// Declarations for functions to access a Thread Safe Counter
//
// Author: Phil Howard

// Reset the thread safe counter to zero
void tsc_reset(long value);

// Increment the thread safe counter. 
// Return the value after the increment.
long  tsc_increment();

// Decrement the thread safe counter.
// Return the value after the decrement.
long  tsc_decrement();

// return the value of the thread safe counter
long  tsc_value();

