#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

int newSocket(int port)
{
    //new TCP socket
    int FD = socket(AF_INET, SOCK_STREAM, 0);
    //set my address info
    struct sockaddr_in myAddr;
    memset(&myAddr, 0, sizeof(myAddr));
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myAddr.sin_port = htons(port);
    char *errorMsg;
    if(bind(FD, (struct sockaddr *)&myAddr, sizeof(myAddr)) != 0)
    {
        asprintf(&errorMsg, "FATAL_ERROR: Unable to bind to port %d\n", port);
        Fatal_Error(errorMsg);
        free(errorMsg);
    }
    if(listen(FD, 5) != 0)
    {
        asprintf(&errorMsg, "FATAL_ERROR: Unable to listen to port %d\n", port);
        Fatal_Error(errorMsg);
        free(errorMsg);
    }
    return FD;
}

