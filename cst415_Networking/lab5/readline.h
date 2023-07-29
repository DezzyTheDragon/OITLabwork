#pragma once

typedef void *Line_Reader_t;

//***********************************************************
// Functions for reading a line at a time from a file descriptor
//
// Author: Phil Howard

// ************************************************
// Prepare for reading lines from a file descriptor.
// This function must be called prior to readline.
// Thread safety: Thread safe provided two threads don't use the same fd
//                or Line_Reader_t at the same time
// Returns: a pointer to a Line_Reader_t on success. NULL on failure.
Line_Reader_t readline_open(int fd);

// ************************************************
// Clean up after all calls to readline for this fd are complete.
// This call will close the reader, 
//     it will NOT close the underlying file descriptor
//
// If readline_close is not called on reader, a memory leak will result.
//
// Thread safety: Thread safe provided two threads don't use the same
//                Line_Reader_t at the same time
//  Returns: zero on success
int readline_close(Line_Reader_t reader);

// ************************************************
// read a line of text from the Line_Reader_t
// Behavior is the same as fgets.
//
// Must call readline_open prior to calling readline.
//
// Thread safety: Thread safe provided two threads don't use the same 
//                Line_Reader_t at the same time
// Returns: see fgets
char *readline(char *buff, ssize_t size, Line_Reader_t reader);

// ************************************************
// read a line of text from the Line_Reader_t
// Behavior is the same as fgets, but only waits timeout seconds.
// If a complete line has not been received within timeout seconds, any data
// that has been received will be returned. buff will be null terminated.
//
// Must call readline_open prior to calling readline.
//
// Thread safety: Thread safe provided two threads don't use the same 
//                Line_Reader_t at the same time
// Returns: see fgets
char *readline_timed(char *buff, ssize_t size, Line_Reader_t reader, int timeout);

// ************************************************
// Write a string to a Line_Reader
//
// Thread safety: Thread safe provided two threads don't use the same 
//                Line_Reader_t at the same time
// Returns: the number of characters written
int writestr(char *buff, Line_Reader_t reader);

//****************************************************
// perform a timeout limited read on a file descriptor.
// Timeout is in seconds
//
// Thread safety: Thread safe provided two threads don't use the same fd
//              (including an fd that's part of a Line_Reader_t) at the
//              same time.
// Note: If the fd passed to this routine is also associated with a 
//              Line_Reader_t, the behavior of the Line_Reader_t is undefined
//              following the call to timed_read.
//
// Returns: 
//      >=0 on success
//      -1 on failure
//      -2 on timeout
int timed_read(int fd, void *buff, int size, int timeout);
