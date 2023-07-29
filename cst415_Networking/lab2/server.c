#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"
#include "encode.h"
#include "addr2str.h"
#include "handlerequest.h"
//#include "list.h"

//Server that requests and uses port from nameserver

int g_other_port = 50000;

int g_program_port;
int g_startingPort;
int g_endingPort;
int g_timeout;

//Parse command line arguments
void parse_opts(int argc, char **argv)
{
    int opt;
    while((opt = getopt(argc, argv, "p:s:e:t:")) != -1)
    {
        switch(opt)
        {
            case 'p':
                g_program_port = atoi(optarg);
                break;
            case 's':
                g_startingPort = atoi(optarg);
                break;
            case 'e':
                g_endingPort = atoi(optarg);
                break;
            case 't':
                g_timeout = atoi(optarg);
                break;
        }
    }
}

int main(int argc, char **argv)
{
    //Default values for command line args
    g_program_port = 30000;
    g_startingPort = 40000;
    g_endingPort = 40099;
    g_timeout = 300;
    parse_opts(argc, argv);
    //open specified port
    int fd = open_socket();
    //check to make sure port was successfully opend
    if(fd == -1)
    {
        //Bail if unable to bind to socket
        printf("Server failed to open on port %i", g_program_port);
        exit(1);
    }
    //Variable to allow a packet to stop nameserver
    int running = 1;
    while(running)
    {
        //Address of the requesting machine
        struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
        //Request
        request_t request;
        memset(&request, 0, sizeof(request_t));
        //Get the request and save the address
        get_data(fd, &request, address);
        //Decode the request
        request_t *dec = decode(&request, &request);
        //Start formulating response
        request_t response;
        memset(&response, 0, sizeof(request_t));
        response.msg_type = MSG_TYPE_RESPONSE;
        //variable to store port number to send back
        int port = 0;
        //Check to make sure packet is valid
        if(dec != NULL)
        {
            //Check if ports have been timed out
            checkTimeout();
            //Packet is valid and handle the request
            int msg = request.msg_type;
            switch(msg)
            {
                case MSG_TYPE_REQUEST_PORT:
                    printf("Recived request port msg\n");
                    port = request_port(request);
                    if(port != -1)
                    {
                        response.port = port;
                        response.status = STATUS_SUCCESS;
                        printf("Giving port: %i\n", port);
                    }
                    else
                    {
                        response.status = STATUS_ALL_PORTS_BUSY;
                    }
                    break;
                case MSG_TYPE_LOOKUP_PORT:
                    printf("Recived lookup port msg\n");
                    port = lookup_port(request);
                    if(port != -1)
                    {
                        response.port = port;
                        response.status = STATUS_SUCCESS;
                    }
                    else
                    {
                        response.status = STATUS_SERVICE_NOT_FOUND;
                    }
                    break;
                case MSG_TYPE_KEEP_ALIVE:
                    printf("Recived keep alive msg\n");
                    keep_alive(request);
                    break;
                case MSG_TYPE_CLOSE_PORT:
                    printf("Recived close port msg\n");
                    close_port(request);
                    break;
                case MSG_TYPE_PORT_DEAD:
                    printf("Recived port dead msg\n");
                    port_dead(request);
                    break;
                case MSG_TYPE_STOP:
                    printf("Recived stop msg\n");
                    //running = 0;
                    break;
                case MSG_TYPE_RESPONSE:
                    printf("Recived response msg\n");
                    break;
            }
        }
        else
        {
            printf("Recived invalid request\n");
        }
    

        //Finish formulating response
        strcpy(response.service_name, "DD_Nameserver");
        //Send response
        send_data(fd, &response, address, sizeof(struct sockaddr_in));

        free(address);
    }
    //clean up
    printf("Stopping nameserver\n");
    cleanup();
    close(fd);
    return 0;
}
