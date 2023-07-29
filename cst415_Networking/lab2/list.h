#include <time.h>
#include "nameserver.h"
typedef struct serviceT
{
    struct serviceT* next;
    char service_name[MAX_SERVICE_NAME_LEN];
    int port;
    time_t timeout;
} service_t;

//find a service and return port number
int findService(char* name);
//add a new service to the list
void addService(char* name, time_t time, int port);
//remove a service from the list
void removeService(char* name);
//reset the timeout for specified service
void resetTimeout(char* name, time_t newTime);
//remove any timed out ports
void clearTimeout(time_t currentTime);
//find the lowest port number in the list
int getLowestPort();
//remove everything from the list
void clearList();
