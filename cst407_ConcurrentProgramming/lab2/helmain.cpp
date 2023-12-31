//**************************************************
// Binary tree test program
//
// Author: Phil Howard

#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <atomic>

#include "cbinary.h"
#include "usec.h"
#include "tsList.h"

// Global variables set by command line options
static int g_tree_size = 1024;
static int g_n_threads = 1;
static int g_insert_percent = 10;
static int g_delete_percent = 10;
static int g_do_list = 0;
static int64_t g_duration = 1000000;
static int64_t g_num_operations = 1000;
static int64_t g_between_tx_delay = 1;
static int64_t g_lookup_delay = 1;
static int g_lock_type = 0;

typedef struct
{
    uint64_t    num_operations;
    uint64_t    num_inserts;
    uint64_t    num_deletes;
    uint64_t    num_lookups;
    uint64_t    num_traversals;
    pthread_t   tid;
    uint64_t    between_tx_delay;
    uint64_t    lookup_delay;
    //tree_t      tree;
    tsList      *list;
    int         index;
    int         insert_percent;
    int         delete_percent;
    int         initial_size;
    int         stuff[50];
} thread_data_t;

//***********************************************
static void parse_args(int argc, char **argv)
{
    int opt;

    while ( (opt = getopt(argc, argv, "s:n:i:d:N:t:b:hl:L") ) != -1)
    {
        switch (opt)
        {
            case 'h':
                printf("%s\n"
                       "-h print this help message and exit\n"
                       "-s <start tree size>\n"
                       "-n <number of threads>\n"
                       "-i <inserts per 1000>\n"
                       "-d <deletes per 1000>\n"
                       "-N Num operations\n"
                       "-t <time per lookup in usecs>\n"
                       "-b <time between transactions in usecs>\n"
                       "-l <lock type> 0=none, 1=one, 2=per node\n"
                       "-L Do a list not a tree\n",
                       argv[0]);
                exit(1);
            case 's':
                // size
                g_tree_size = atoi(optarg);
                break;
            case 'n':
                // nthreads
                g_n_threads = atoi(optarg);
                break;
            case 'i':
                // insert %
                g_insert_percent = atoi(optarg);
                break;
            case 'd':
                // delete %
                g_delete_percent = atoi(optarg);
                break;
            case 'N':
                // time to run
                g_num_operations = atol(optarg);
                break;
            case 'w':
                // time to run
                g_duration = atoi(optarg) * 1000000000L;
                break;
            case 't':
                // time per lookup
                g_lookup_delay = atoi(optarg) ;
                break;
            case 'b':
                // time between each transacion
                g_between_tx_delay = atoi(optarg) ;
                break;
            case 'l':
                // lock type
                g_lock_type = atoi(optarg);
                break;
            case 'L':
                // do list, not tree
                g_do_list = 1;
                break;
        }
    }
}
//***********************************************
static void *Thread_Func(void *arg)
{
    thread_data_t *args = (thread_data_t*)arg;

    // init random number seed
    unsigned int seed = (unsigned int)usecs();
    int option;
    uint64_t delay;
    int value;

    //printf("Thread %d seed %d\n", args->index, seed);
    args->num_inserts = 0;
    args->num_deletes = 0;
    args->num_lookups = 0;
    args->num_traversals = 0;

    args->num_traversals = nsecs();

    // process until told to stop
    for (uint64_t ii=0; ii<args->num_operations; ii++)
    {
        delay = rand_r(&seed) % args->between_tx_delay;
        usec_delay(delay);
        
        option = rand_r(&seed) % 1000;
        if (option < args->insert_percent)
        {
            value = rand_r(&seed)%(2*g_tree_size);
            //Tree_Insert(args->tree, value);
            args->list->Insert(value);
            args->num_inserts++;
        }
        else if (option < (args->insert_percent + args->delete_percent))
        {
            value = rand_r(&seed)%(2*g_tree_size);
            //Tree_Delete(args->tree, value);
            args->list->RemoveBegining();
            args->num_deletes++;
        }
        else
        {
            //element_t elem;
            t_ListNode *elem;
            value = rand_r(&seed)%(2*g_tree_size);
            //elem = Tree_Lookup(args->tree, value);
            elem = args->list->Lookup(value);
            args->num_lookups++;
            delay = rand_r(&seed) % args->lookup_delay;
            usec_delay(delay);
            //Element_Release(elem);
            args->list->ElementRelease(elem);
        }
    }

    args->num_traversals = nsecs() - args->num_traversals;

    return NULL;
}

//***********************************************
// Count the nodes in a tree
// NOTE: g_node_count must be set to zero prior to the travers
int64_t g_node_count;
//void Count_Nodes(element_t element)
void Count_Nodes(int value)
{
    g_node_count++;
}
//***********************************************
int main(int argc, char **argv)
{
    //tree_t tree;

    parse_args(argc, argv);
    //tree = Tree_Init(g_lock_type);
    tsList *list = new tsList(g_lock_type);
    int value;

    unsigned int seed = (unsigned int)usecs();
    for (int ii=0; ii<g_tree_size; ii++)
    {
        if (g_do_list)
            value = ii+5;
        else
            value = rand_r(&seed)%(2*g_tree_size);

        //Tree_Insert(tree, value);
        list->Insert(value);
    }

    thread_data_t *threads;
    threads = (thread_data_t*)malloc(g_n_threads*sizeof(thread_data_t));


    g_node_count = 0;
    //Tree_Traverse(tree, Count_Nodes);
    list->Traverse(Count_Nodes);

    printf("There were %ld nodes in the tree at the start of the run\n\n", g_node_count);

    for (int ii=0; ii<g_n_threads; ii++)
    {
        //threads[ii].tree = tree;
        threads[ii].index = ii+1;
        threads[ii].insert_percent = g_insert_percent;
        threads[ii].delete_percent = g_delete_percent;
        threads[ii].num_operations = g_num_operations;
        threads[ii].between_tx_delay = g_between_tx_delay;
        threads[ii].lookup_delay = g_lookup_delay;
        threads[ii].initial_size = g_tree_size;
        threads[ii].list = list;
        pthread_create(&threads[ii].tid, NULL, Thread_Func, &threads[ii]);
    }

    uint64_t num_inserts = 0;
    uint64_t num_deletes = 0;
    uint64_t num_lookups = 0;
    uint64_t num_traversals = 0;

    for (int ii=0; ii<g_n_threads; ii++)
    {
        pthread_join(threads[ii].tid, NULL);
        printf("Thread %3d stats: %10ld %10ld %10ld %10ld\n",
                ii+1, threads[ii].num_inserts, threads[ii].num_deletes,
                threads[ii].num_lookups, threads[ii].num_traversals);

        num_inserts += threads[ii].num_inserts;
        num_deletes += threads[ii].num_deletes;
        num_lookups += threads[ii].num_lookups;
        num_traversals += threads[ii].num_traversals;
    }

    printf("\n");
    printf("Thread Tot stats: %10ld %10ld %10ld %10ld\n",
            num_inserts, num_deletes, num_lookups, num_traversals); 

    free(threads);

    g_node_count = 0;
    //Tree_Traverse(tree, Count_Nodes);
    list->Traverse(Count_Nodes);

    printf("There were %ld nodes in the tree at the end of the run\n", g_node_count);

    //Tree_Destroy(tree);
    delete list;

    return 0;
}

