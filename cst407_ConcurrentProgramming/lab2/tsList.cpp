#include "tsList.h"
#include <iostream>

tsList::tsList(int lock)
{
    //printf("Constructor\n");
    lockType = lock;
    printf("Lock is %d\n", lockType);
    head = nullptr;
    tail = nullptr;
    rwLock = PTHREAD_RWLOCK_INITIALIZER;
}

tsList::~tsList()
{
    //printf("Destructor not implemented\n");
    //tranverse list and destroy each node
    
    t_ListNode *travel = head;
    t_ListNode *destroy = travel;
    while(travel != nullptr)
    {
        destroy = travel;
        travel = travel->next;
        delete destroy;
    }
    
}

void tsList::PushFront(int val)
{
    //printf("Add %d to front\n", val);
    //Check lock type
    //lock/write lock
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_lock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_wrlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
    t_ListNode *newNode = new t_ListNode;
    newNode->value = val;
    if(head == nullptr)
    {
        newNode->next = nullptr;
        newNode->last = nullptr;
        tail = newNode;
    }
    else
    {
        newNode->next = head;
        newNode->last = nullptr;
        head->last = newNode;
    }
    head = newNode;
    //unlock
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_unlock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_unlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
}

void tsList::PushBack(int val)
{
    //printf("Add %d to back\n", val);
    //check lock
    //lock/write lock
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_lock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_wrlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
    t_ListNode *newNode = new t_ListNode;
    newNode->value = val;
    if(tail == nullptr)
    {
        newNode->next = nullptr;
        newNode->last = nullptr;
        head = newNode;
    }
    else
    {
        newNode->next = nullptr;
        newNode->last = tail;
        tail->next = newNode;
    }
    tail = newNode;
    //unlock
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_unlock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_unlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
}

void tsList::Insert(int val)
{
    //printf("Inserting %d\n", val);
    //Check lock type
    //lock/write lock
    //printf("Insert lock type is %d\n", lockType);
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_lock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_wrlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
    t_ListNode *newNode = new t_ListNode;
    newNode->value = val;
    if(head == nullptr)
    {
        newNode->next = nullptr;
        newNode->last = nullptr;
        tail = newNode;
        head = newNode;
    }
    else
    {
        bool added = false;
        t_ListNode *travel = head;
        while(!added && travel != nullptr)
        {
            if(travel->value > val)
            {
                newNode->last = travel->last;
                newNode->next = travel; 
                travel->last = newNode;
                if(newNode->last != nullptr)
                {
                    newNode->last->next = newNode;
                }
                added = true;
            }
            travel = travel->next;
        }
        if(!added)
        {
            if(tail == nullptr)
            {
                newNode->next = nullptr;
                newNode->last = nullptr;
                head = newNode;
            }
            else
            {
                newNode->next = nullptr;
                newNode->last = tail;
                tail->next = newNode;
            }
            tail = newNode;
        }
    }
    //unlock
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_unlock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_unlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
}

t_ListNode* tsList::Lookup(int val)
{
    //printf("Finding node with %d not implemented\n", val);
    //Check lock type
    //lock/read lock
    //printf("lookup lock %d\n", lockType);
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_lock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_rdlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
    t_ListNode *target = head;
    bool found = false;
    while(target != nullptr && !found)
    {
        if(target->value == val)
        {
            found = true;
        }
        else
        {
            target = target->next;
        }
    }
    //unlock
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_unlock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_unlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
    return target;
}

int tsList::ElementValue(t_ListNode *node)
{
    //printf("Getting value in node not implemented\n");
    //check lock type
    //lock/read lock
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_lock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_rdlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
    int ret = node->value;
    //unlock
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_unlock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_unlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
    return ret;
}

void tsList::ElementRelease(t_ListNode *node)
{
    //printf("Freeing the locks on node not implemented\n");
    //
}

void tsList::Traverse(void (*func)(int))
{
    //printf("Tranversing list and executing function not implemented\n");
    //check lock type
    //lock/write lock
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_lock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_rdlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
    t_ListNode *travel = head;
    while(travel != nullptr)
    {
        func(travel->value);
        travel = travel->next;
    }
    //unlock
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_unlock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_unlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
}

void tsList::RemoveBegining()
{
    //printf("Remove from begining not implemented\n");
    //check lock type
    //lock/write lock
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_lock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_wrlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
    t_ListNode *destroy = head;
    head = head->next;
    delete destroy;
    //unlock
    switch(lockType)
    {
        case LOCK_MODE_COARSE:
            pthread_mutex_unlock(&coarseLock);
            break;
        case LOCK_MODE_READ_WRITE:
            pthread_rwlock_unlock(&rwLock);
            break;
        case LOCK_MODE_FINE:
            break;
    }
}

t_ListNode *tsList::Peek()
{
    //printf("Return pointer to front");
    return head;
}



