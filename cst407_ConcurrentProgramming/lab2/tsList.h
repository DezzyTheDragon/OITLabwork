#pragma once
#include <pthread.h>

#define LOCK_MODE_NONE          1
#define LOCK_MODE_COARSE        2
#define LOCK_MODE_READ_WRITE    3
#define LOCK_MODE_FINE          4


typedef struct node{
    int value;
    node *next;
    node *last;
} t_ListNode;

class tsList
{
    public:
        tsList(int lock);
        ~tsList();
        void PushFront(int val);
        void PushBack(int val);
        void Insert(int val);
        t_ListNode *Lookup(int val);
        int ElementValue(t_ListNode *node);
        void ElementRelease(t_ListNode *node);
        void Traverse(void (*func)(int));
        void RemoveBegining();
        t_ListNode *Peek();

    private:
        t_ListNode *head;
        t_ListNode *tail;
        int lockType;
        pthread_mutex_t coarseLock;
        pthread_rwlock_t rwLock;
};
