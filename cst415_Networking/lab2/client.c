#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <unistd.h>
#include "socket.h"
#include "nameserver.h"
#include "encode.h"
//This is the client program for lab2

int g_other_port = 50000;
int g_program_port = 0;

int newServiceTest(int fd, struct sockaddr_in *address)
{
    int ret = 0;
    request_t newService1;
    request_t newService2;
    memset(&newService1, 0, sizeof(request_t));
    memset(&newService2, 0, sizeof(request_t));
    newService1.msg_type = MSG_TYPE_REQUEST_PORT;
    strcpy(newService1.service_name, "Dezzy1");
    newService2.msg_type = MSG_TYPE_REQUEST_PORT;
    strcpy(newService2.service_name, "Dezzy2");
    //send request 1
    send_data(fd, &newService1, address, sizeof(struct sockaddr_in));
    //get response for request 1
    struct sockaddr_in *other = malloc(sizeof(struct sockaddr_in));
    request_t response;
    memset(&response, 0, sizeof(request_t));
    get_data(fd, &response, other);
    decode(&response, &response);
    
    if(response.msg_type != MSG_TYPE_RESPONSE && response.status != STATUS_SUCCESS)
    {
        ret = -1;
    }
    else
    {
        printf("\tRecived port: %i\n", response.port);
    }
    //send request 2
    send_data(fd, &newService2, address, sizeof(struct sockaddr_in));
    //get response for request 2
    memset(&response, 0, sizeof(request_t));
    get_data(fd, &response, other);
    decode(&response, &response);
    if(response.msg_type != MSG_TYPE_RESPONSE && response.status != STATUS_SUCCESS)
    {
        ret = -1;
    }
    else
    {
        printf("\tRecived port: %i\n", response.port);
    }
    return ret;
}

int findServiceTest(int fd, struct sockaddr_in *address)
{
    int ret = 0;
    request_t findService1;
    request_t findService2;
    request_t findService3;
    request_t response;
    struct sockaddr_in *other = malloc(sizeof(struct sockaddr_in));
    memset(&findService1, 0, sizeof(request_t));
    memset(&findService2, 0, sizeof(request_t));
    memset(&findService3, 0, sizeof(request_t));
    memset(&response, 0, sizeof(request_t));
    findService1.msg_type = MSG_TYPE_LOOKUP_PORT;
    findService2.msg_type = MSG_TYPE_LOOKUP_PORT;
    findService3.msg_type = MSG_TYPE_LOOKUP_PORT;
    strcpy(findService1.service_name, "Dezzy1");
    strcpy(findService2.service_name, "Dezzy2");
    strcpy(findService3.service_name, "Dezzy3");
    //find service 1
    send_data(fd, &findService1, address, sizeof(struct sockaddr_in));
    //get response
    get_data(fd, &response, other);
    decode(&response, &response);
    if(response.msg_type != MSG_TYPE_RESPONSE && response.status != STATUS_SUCCESS)
    {
        ret = -1;
    }
    else
    {
        printf("\tService was found with port %i\n", response.port);
    }
    //find service 2
    send_data(fd, &findService2, address, sizeof(struct sockaddr_in));
    //get resonse
    memset(&response, 0, sizeof(request_t));
    get_data(fd, &response, other);
    decode(&response, &response);
    if(response.msg_type != MSG_TYPE_RESPONSE && response.status != STATUS_SUCCESS)
    {
        ret = -1;
    }
    else
    {
        printf("\tService was found with port %i\n", response.port);
    }
    //find non-existant service 3
    send_data(fd, &findService3, address, sizeof(struct sockaddr_in));
    //get response 3
    memset(&response, 0, sizeof(request_t));
    get_data(fd, &response, other);
    decode(&response, &response);
    if(response.msg_type != MSG_TYPE_RESPONSE && response.status != STATUS_SERVICE_NOT_FOUND)
    {
        ret = -1;
    }
    else
    {
        printf("\tService was not found... Good\n");
    }
    free(other);
    return ret;
}

int removeTest(int fd, struct sockaddr_in *address)
{
    int ret = 0;
    request_t remove;
    request_t check;
    request_t response;
    struct sockaddr_in *other = malloc(sizeof(struct sockaddr_in));
    memset(&remove, 0, sizeof(request_t));
    memset(&check, 0, sizeof(request_t));
    memset(&response, 0, sizeof(request_t));
    strcpy(remove.service_name, "Dezzy2");
    strcpy(check.service_name, "Dezzy2");
    remove.msg_type = MSG_TYPE_CLOSE_PORT;
    check.msg_type = MSG_TYPE_LOOKUP_PORT;
    remove.port = 40001;
    send_data(fd, &remove, address, sizeof(struct sockaddr_in));
    send_data(fd, &check, address, sizeof(struct sockaddr_in));
    get_data(fd, &response, other);
    if(response.status != STATUS_SERVICE_NOT_FOUND)
    {
        ret = -1;
    }
    else
    {
        printf("\tService was removed\n");
    }
    free(other);
    return ret;
}

int keepAliveTest(int fd, struct sockaddr_in *address)
{
    request_t refresh;
    memset(&refresh, 0, sizeof(request_t));
    strcpy(refresh.service_name, "Dezzy1");
    refresh.msg_type = MSG_TYPE_KEEP_ALIVE;
    send_data(fd, &refresh, address, sizeof(struct sockaddr_in));

    return 0;
}

void stopTest(int fd, struct sockaddr_in *address)
{
    request_t stopRequest;
    memset(&stopRequest, 0, sizeof(request_t));
    stopRequest.msg_type = MSG_TYPE_STOP;
    send_data(fd, &stopRequest, address, sizeof(struct sockaddr_in));
}

int main(int argc, char **argv)
{
    int opt = getopt(argc, argv, "p:");
    if(opt == 'p')
    {
        g_other_port = atoi(optarg);
    }

    //Open socket binding 
    int fd = open_socket();
    //Create the address structure for the nameserver
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    build_server_addr(address);

    //run tests
    printf("Test 1:\n");
    if(newServiceTest(fd, address) != 0)
    {
        printf("Failed\n");
    }
    else
    {
        printf("Passed\n");
    }
    
    printf("Test 2:\n");
    if(findServiceTest(fd, address) != 0)
    {
        printf("Failed\n");
    }
    else
    {
        printf("Passed\n");
    }

    printf("Test 3:\n");
    if(removeTest(fd, address) != 0)
    {
        printf("Failed\n");
    }
    else
    {
        printf("Passed\n");
    }

    printf("Test 4:\n");
    keepAliveTest(fd, address);

    printf("Stop nameserver\n");
    stopTest(fd, address);

    //Create the request to be sent
    /*
    request_t request;
    memset(&request, 0, sizeof(request_t));
    request.msg_type = MSG_TYPE_LOOKUP_PORT;
    request.status = STATUS_SUCCESS;
    strcpy(request.service_name, "Destiny");
    //Send the request to nameserver
    send_data(fd, &request, address, sizeof(struct sockaddr_in));
    //Get response from nameserver
    //Throw away address to get function to work
    struct sockaddr_in *other = malloc(sizeof(struct sockaddr_in));
    request_t response;
    memset(&response, 0, sizeof(request_t));
    get_data(fd, &response, other);
    free(other);
    printf("Response from nameserver:\nmsg_type: %i\nstatus: %i\n",
            response.msg_type, response.status);
    */
    //Clean stuff up
    free(address);
    close(fd);
    return 0;
}
