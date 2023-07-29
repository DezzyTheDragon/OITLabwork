#include "list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern service_t* serv_list;
extern int g_startingPort;
extern int g_endingPort;

//go through the service list and return the port of a matching
//service. If no service was found return -1

int findService(char* name)
{
    if(serv_list == NULL)
    {
        return -1;
    }
    int port = -1;
    service_t* target = serv_list;
    while(target != NULL)
    {
        if(strcmp(target->service_name, name) == 0)
        {
            port = target->port;
            break;
        }
        target = target->next;
    }
    return port;
}

//add service in port order
void addService(char* name, time_t time, int port)
{
    service_t *newService = malloc(sizeof(service_t));
    memset(newService, 0, sizeof(service_t));
    strcpy(newService->service_name, name);
    newService->port = port;
    newService->timeout = time;
    if(serv_list == NULL)
    {
        serv_list = newService;
    }
    else
    {
        service_t *target = serv_list;
        service_t *trail = serv_list;
        int assigned = 0;
    
        while(target != NULL)
        {
            if(target->port > port)
            {
                newService->next = serv_list;
                serv_list = newService;
                assigned = 1;
                break;
            }
            if(target->port > trail->port)
            {
                trail->next = newService;
                newService->next = target;
                assigned = 1;
                break;
            }
            trail = target;
            target = target->next;
        }
        if(!assigned)
        {
            trail->next = newService;
            newService->next = NULL;
        }
    }
}


//remove service from list
void removeService(char* name)
{
    service_t *target = serv_list;
    service_t *trail = serv_list;
    while(serv_list != NULL)
    {
        if(strcmp(target->service_name, name))
        {
            if(serv_list == target)
            {
                serv_list = serv_list->next;
            }
            else
            {
                trail->next = target->next;
                target->next = NULL;
            }
            printf("Removing Service from list\n");
            free(target);
            break;
        }
        trail = target;
        target = target->next;
    }
}

void resetTimeout(char* name, time_t time)
{
    service_t* target = serv_list;
    while(target != NULL)
    {
        if(strcmp(target->service_name, name))
        {
            target->timeout = time;
            break;
        }
    }
}

void clearTimeout(time_t currentTime)
{
    service_t* target = serv_list;
    service_t* trail = serv_list;
    while(target != NULL)
    {
        if(target->timeout < currentTime)
        {
            service_t* del = target;
            if(serv_list == del)
            {
                serv_list = serv_list->next;
            }
            else
            {
                trail->next = target->next;
                target = target->next;
                del->next = NULL;
            }
            free(del);
        }
        trail = target;
        target = target->next;
    }
}

int getLowestPort()
{
    int port = -1;
    if(serv_list == NULL)
    {
        port = g_startingPort;
    }
    else
    {
        service_t* target = serv_list;
        service_t* trail = serv_list;
        int found = 0;
        if(serv_list->port > g_startingPort)
        {
            port = g_startingPort;
        }
        else
        {
            while(target != NULL)
            {
                if(target->port - trail->port > 1)
                {
                    port = trail->port + 1;
                    found = 1;
                    break;
                }
                trail = target;
                target = target->next;
            }
            if(!found)
            {
                port = trail->port + 1;
                if(port > g_endingPort)
                {
                    port = -1;
                }
            }
        }
    }
    return port;
}

void clearList()
{
    service_t* target = serv_list;
    while(target != NULL)
    {
        printf("Cleaning up list\n");
        service_t* del = target;
        target = target->next;
        free(del);
    }
}

