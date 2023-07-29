#include "nameserver.h"
//Handle the port request
//returns port number on success
//return -1 on fail
int request_port(request_t request);
//Look up the port for a service
//returns port number on success
//return -1 on fail
int lookup_port(request_t request);
//Refresh a service timeout
void keep_alive(request_t request);
//Close the specified port
void close_port(request_t request);
//Add a dead port to the list
void port_dead(request_t request);
//Wrapper for list clean up
void cleanup();
//Iterate through the list and clear any timed out
//ports
void checkTimeout();
