//**************************************************
// Functions to access the CST 415 Nameserver
//
// Author: Phil Howard

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "utils.h"
#include "nameserver.h"
#include "getport.h"
#include "encode.h"

static int g_nameserver = -1;
static struct sockaddr_in g_server_addr;

//*************************************************
// Setup access to the nameserver.
// This function must be called before any of the others
// Thread safety: None
int setup_ns(const char *host, int port)
{
    // see if we've already done this
    if (g_nameserver >= 0) return 0;

    g_nameserver = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_nameserver < 0) Fatal_Error("Unable to create service socket");

    struct sockaddr_in myaddr;
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(0);        // request a port

    if (bind(g_nameserver, (struct sockaddr *)&myaddr, sizeof(myaddr)) != 0)
    {
        Fatal_Error("Unable to bind to port for communicating with ns\n");
    }

    struct addrinfo hints;
    struct addrinfo *addr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    if (getaddrinfo(host, NULL, &hints, &addr) != 0)
    {
        perror("Error getting address info: ");
        Fatal_Error("Error getting address info\n");
    }

    memcpy(&g_server_addr, addr->ai_addr, addr->ai_addrlen);
    g_server_addr.sin_port = htons(port);

    freeaddrinfo(addr);

    return 0;
}
//*************************************************
// Wrapper that handles the communication with the nameserver
// Thread safety: none
static int do_request(const char *service_name, int type, int port, 
        uint32_t *timeout)
{
    request_t request;
    request_t result;
    int status;
    int recvlen;

    setup_ns(NULL, 0);

    memset(&request, 0, sizeof(request));
    memset(&result, 0, sizeof(result));
    if (timeout != NULL) *timeout = 0;

    request.msg_type = type;
    strncpy(request.service_name, service_name, sizeof(request.service_name));
    request.port = htons(port);
    request.status = STATUS_SUCCESS;
    request.timeout = 0;

    status = sendto(g_nameserver, &request, sizeof(request), 0, 
        (struct sockaddr *)&g_server_addr, sizeof(g_server_addr));
    if (status < 0) 
    {
        //perror("Error sending msg: ");
        return -1;
    }

    recvlen = recvfrom(g_nameserver, &result, sizeof(result), 0, NULL, NULL);
    if (recvlen < 0) 
    {
        //Fatal_Error("Error on service socket");
        return -2;
    }

    if (recvlen != sizeof(request)) 
    {
        //fprintf(stderr, "Received an unrecognized packet\n");
        return -3;
    }

    if (decode(&result, &result) == NULL)
    {
        //fprintf(stderr, "Received an invalid response from the nameserver\n");
        return -4;
    }

    if (result.status != STATUS_SUCCESS)
    {
        //fprintf(stderr, "Received an unrecognized packet\n");
        return -5;
    }

    if (timeout != NULL) *timeout = result.timeout;

    return result.port;
}
//*************************************************
// Thread safety: none
int request_port(const char *service_name, uint32_t *timeout)
{
    return do_request(service_name, MSG_TYPE_REQUEST_PORT, 0, timeout);
}
//*************************************************
// Thread safety: None
int lookup_port(const char *service_name)
{
    return do_request(service_name, MSG_TYPE_LOOKUP_PORT, 0, NULL);
}
//*************************************************
// Thread safety: None
int release_port(const char *service_name, int port)
{
    return do_request(service_name, MSG_TYPE_CLOSE_PORT, port, NULL);
}
//*************************************************
// Thread safety: None
int keep_alive_port(const char *service_name, int port)
{
    return do_request(service_name, MSG_TYPE_KEEP_ALIVE, port, NULL);
}
