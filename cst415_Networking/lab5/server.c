#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "getport.h"
#include "utils.h"
#include "connect.h"
#include "addr2str.h"
#include "timed_accept.h"
#include "readline.h"

//struct for containing the "settings" of the server
typedef struct {
    int nameserverPort;
    char serviceName[10];
    char hostName[20];
    int timeout;
    int maxClients;
} t_serverInfo;

//Parse the command line arguments to change
//server settings
//Takes command line arguments and a pointer to
//serverInfo struct
//parseArg will change serverInfo strutc according to
//the arguments given. If no arguments are given
//serverInfo is unchanged
void parseArg(int argc, char** argv, t_serverInfo *info)
{
    //handle server arguments
    //-h        help
    //-p <#>    nameserver port
    //-n <str>  service name
    //-t <#>    timeout
    //-c <#>    concurrent clients

    int opt;
    int TO;
    while((opt = getopt(argc, argv, "hp:n:t:c:")) != -1)
    {
        switch(opt)
        {
            case 'h':
                //help and exit program
                printf("Help\n\
                        -h:\t\thelp and exit\n\
                        -p <number>:\tport number of the nameserver\n\
                        -n <name>:\tname of ther service for the nameserver\n\
                        -t <number>:\tthe timeout, any value less than or\
                        equal to 0 will default to 5\n\
                        -c <number>:\tthe number of concurrent clients\n");
                exit(0);
                break;
            case 'p':
                //name server port
                info->nameserverPort = atoi(optarg);
                break;
            case 'n':
                //service name
                memcpy(info->serviceName, optarg, sizeof(info->serviceName));
                break;
            case 't':
                //timeout
                TO = atoi(optarg);
                if(TO <= 0)
                {
                    TO = 5;
                }
                info->timeout = TO;
                break;
            case 'c':
                //concurrent clients
                info->maxClients = atoi(optarg);
                break;
        }
    }
}

//Contact nameserver and get a port for
//this server to use
//takes server info struct
//returns a port number
int getServerPort(t_serverInfo serverInfo)
{
    //get port for server to run on
    if(setup_ns(serverInfo.hostName, serverInfo.nameserverPort))
    {
        //Fatal error
        Fatal_Error("FATAL_ERROR: Unable to connect to nameserver\n");
    }
    uint32_t *temp = NULL;
    int serverPort = request_port(serverInfo.serviceName, temp);
    if(serverPort < 0)
    {
        //Error, service may already exist
        printf("ERROR: Unable to request new port, checking for existing port\n");
        serverPort = lookup_port(serverInfo.serviceName);
        if(serverPort < 0)
        {
            //Fatal error
            Fatal_Error("FATAL_ERROR: Unable to get new port\n");
        }
    }
    return serverPort;
}

//Handle client function
void *handleClient(void *ptr)
{
    int fd = *(int *)ptr;
    pthread_detach(pthread_self());
    //just read a bunch of stuff

    char buff[100];
    //char *connected = buff;
    //int running = 1;
    //Line_Reader_t lr = readline_open(fd);
    int readSize = 1;
    //while(connected != NULL && running)
    while(readSize > 0)
    {
        //connected = readline(buff, sizeof(buff), lr);
        readSize = read(fd, buff, sizeof(buff));
        //printf(buff);
    }
    printf("\nSERVER_INFO: Done talking to client %d\n", fd);
    //readline_close(lr);
    close(fd);
    return NULL;
}

//Main function
int main(int argc, char** argv)
{
    //starting server
    t_serverInfo serverInfo;
    serverInfo.nameserverPort = 50000;
    serverInfo.timeout = 30;
    serverInfo.maxClients = 5;
    memset(serverInfo.serviceName, 0, sizeof(serverInfo.serviceName));
    strcpy(serverInfo.serviceName, "DD_Lab5");
    memset(serverInfo.hostName, 0, sizeof(serverInfo.hostName));
    strcpy(serverInfo.hostName, "unix.cset.oit.edu");
    parseArg(argc, argv, &serverInfo);
    printf("SERVER_STARTING:\n\
            \tService_Name:\t\t%s\n\
            \tNameserver_Address:\t%s\n\
            \tNameserver_Port:\t%d\n\
            \tTimeout:\t\t%d\n\
            \tMax_Clients:\t\t%d\n",
            serverInfo.serviceName, serverInfo.hostName, serverInfo.nameserverPort,
            serverInfo.timeout, serverInfo.maxClients);
    //Get port for server
    int serverPort = getServerPort(serverInfo);
    //Open Socket for listening
    int listenFD = newSocket(serverPort);
    //run a keep_alive_massage
    keep_alive_port(serverInfo.serviceName, serverPort);
    //Server startup is finished
    printf("SERVER_INFO: Server running on %s\n", sock2str(listenFD, NULL));
    //calculate timed accept so a keepalive can still be sent
    int TO = serverInfo.timeout - 1;
    if(TO == 0)
    {
        TO = 1;
    }
    while(1)
    {
        //accept connection
        //disregard any incomming data
        int clientFD = timed_accept(listenFD, NULL, NULL, TO);
        keep_alive_port(serverInfo.serviceName, serverPort);
        if(clientFD > 0)
        {
            //Client is connected, check if there is "room"
            //  spin a new thread to handle client
            //if no room
            //  hang up
            printf("SERVER_INFO: Talking to client %d\n", clientFD);
            pthread_t thread;
            pthread_create(&thread, NULL, handleClient, (void *)&clientFD);
        }
        else
        {
            printf("SERVER_INFO: No client, sending keep alive\n");
        }
    }

    //cleanup
    close(listenFD);
    return 0;
}











