//*******************************************************
// lock performance test harness
//
// Author: phil howard

//#ifndef _POSIX_C_SOURCE
//#define _POSIX_C_SOURCE 4
//#endif

#include <unistd.h>
#include <cstdio>
#include <thread>
#include <atomic>
#include <math.h>
#include <sched.h>      // for sched_getcpu()

#include <pthread.h>    // for pthread_self(). std::thread doesn't give numeric value

#include "timer.h"
#include "lock.h"
#include "nolock.h"
#include "spinlock.h"
#include "spinlock2.h"
#include "mutex.h"
#include "rwr_lock.h"
#include "rww_lock.h"

//**********************************************
// data passed to thread
typedef struct
{
    lock_t *lock;
    std::atomic<bool> *run;
    std::atomic<long> *total_reads;
    std::atomic<long> *total_writes;
    bool do_stress_test;
} thread_data_t;

//***********************************************
// Thread function for scalability test
void time_locks(lock_t *lock, std::atomic<bool> *run, 
		std::atomic<long> *read_total, std::atomic<long> *write_total, 
                int write_percent)
{
    long read_count = 0;
    long write_count = 0;

    pthread_t tid = pthread_self();
    unsigned int seed = (unsigned int)tid;

    // print CPU number
    printf("Worker thread on CPU %d\n", sched_getcpu());

    while (!run->load(std::memory_order_relaxed))
    {}

    while (run->load(std::memory_order_relaxed))
    {
        if (rand_r(&seed) % 1000 < write_percent)
        {
            lock->write_lock();
            write_count++;
        }
        else
        {
            lock->read_lock();
            read_count++;
        }

        lock->unlock();
    }

    printf("Thread acquired %ld read locks and %ld write locks\n", 
            read_count, write_count);

    read_total->fetch_add(read_count);
    write_total->fetch_add(write_count);
}

//*********************************************
// Function to start a set of threads
std::thread **start_threads(int num_threads, int write_percent, 
                            thread_data_t *thread_info)
{
    std::thread **threads = new std::thread*[num_threads];

    for (int ii=0; ii<num_threads; ii++)
    {
        threads[ii] = new std::thread(time_locks, 
                thread_info->lock, 
                thread_info->run, 
                thread_info->total_reads, 
                thread_info->total_writes, 
                write_percent);
    }

    return threads;
}

//**************************************************************
// Test harness for locks
int main(int argc, char **argv)
{
    int lock_type = 1;
    int n_threads = 1;
    int n_read_threads = 0;
    int n_write_threads = 0;
    int write_percent = 0;
    bool do_stress_test = false;
    int opt;
    lock_t *lock;
    std::atomic<bool> run(false);
    std::atomic<long> total_reads = {0};
    std::atomic<long> total_writes = {0};


    //************************************************
    // parse command line args
    while ((opt = getopt(argc, argv, "hl:n:sw:R:W:")) > 0)
    {
        switch(opt)
        {
        case 'h':
            printf("%s -h : help\n"
                    "-l <lock type>\n"
                    "  lock types: 0 = no lock\n"
                    "              1 = spinlock\n"
                    "              2 = test and test and set\n"
                    "              3 = c++ mutex\n"
                    "              4 = reader-writer favors reads\n"
                    "              5 = reader-writer favors writes\n"
                    "-n <num read/write threads>\n"
                    "-w <writes/1000>\n"
                    "-R <num read-only threads>\n"
                    "-W <num write-only threads>\n"
                    "-s : do stresss test not scalability test\n"
                    "", argv[0]);
            exit(1);
            break;
        case 'l':
            lock_type = atoi(optarg);
            break;
        case 'n':
            n_threads = atoi(optarg);
            break;
        case 'R':
            n_read_threads = atoi(optarg);
            break;
        case 'W':
            n_write_threads = atoi(optarg);
            break;
        case 's':
            printf("Stress testing not supported in this version\n");
            exit(1);
            //do_stress_test = true;
            break;
        case 'w':
            write_percent = atoi(optarg);
            break;
        }
    }

    // ***********************************************
    // create specified lock
    switch (lock_type)
    {
        case 0:
            lock = new nolock_t();
            break;
        case 1:
            lock = new spinlock_t();
            break;
        case 2:
            lock = new spinlock2_t();
            break;
        case 3:
            lock = new mutex_t();
            break;
        case 4:
            lock = new rwr_lock_t();
            break;
        case 5:
            lock = new rww_lock_t();
            break;
        default:
            lock = new spinlock_t();
            break;
    }

    // Init thread data
    thread_data_t thread_info;
    thread_info.lock = lock;
    thread_info.run = &run;
    thread_info.total_reads = &total_reads;
    thread_info.total_writes = &total_writes;
    thread_info.do_stress_test = do_stress_test;

    // Create the threads
    std::thread **rw_threads;
    std::thread **read_threads;
    std::thread **write_threads;

    rw_threads = start_threads(n_threads, write_percent, &thread_info);
    read_threads = start_threads(n_read_threads, 0, &thread_info);
    write_threads = start_threads(n_write_threads, 1000, &thread_info);

    // print CPU number
    printf("Main thread on CPU %d\n", sched_getcpu());

    // wait for them to start, then run them for 1 second
    timer clock;
    clock.delay(1.0);
    run.store(true, std::memory_order_release);

    clock.delay(1.0);
    run.store(false, std::memory_order_release);

    // wait for threads to finish
    for (int ii=0; ii<n_threads; ii++)
    {
        rw_threads[ii]->join();
    }
    for (int ii=0; ii<n_read_threads; ii++)
    {
        read_threads[ii]->join();
    }
    for (int ii=0; ii<n_write_threads; ii++)
    {
        write_threads[ii]->join();
    }

    printf("Total locks: read %f write %f (millions)\n", 
            total_reads.load()/1000000.0, total_writes.load()/1000000.0);

    return 0;
}
