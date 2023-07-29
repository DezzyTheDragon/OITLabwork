#include <stdio.h>
#include <time.h>
#include "nameserver.h"
#include "list.h"

extern int g_timeout;

service_t* serv_list = NULL;

int request_port(request_t request)
{
    //printf("Port Request\n");
    int port = getLowestPort();
    time_t timeout;
    time_t temp;
    time(&temp);
    timeout = temp + g_timeout;
    addService(request.service_name, timeout, port);

    return port;
}
int lookup_port(request_t request)
{
    printf("Lookup Port\n");
    int port = findService(request.service_name);
    return port;
}
void keep_alive(request_t request)
{
    printf("Keep Alive\n");
    time_t timeout;
    time_t temp;
    time(&temp);
    timeout = temp + g_timeout;
    resetTimeout(request.service_name, timeout);
}
void close_port(request_t request)
{
    printf("Close Port\n");
    if(request.port == lookup_port(request))
    {
        removeService(request.service_name);
    }
    else
    {
        printf("Unauthorized close attempt\n");
    }
}
void port_dead(request_t request)
{
    printf("Port Dead\n");
    //addService("Dead_Port");
}
void cleanup()
{
    printf("Cleaning up list\n");
    clearList();
}
void checkTimeout()
{
    time_t timeout;
    time(&timeout);
    clearTimeout(timeout);
}
