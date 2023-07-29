#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include "encode.h"
#include "util.h"
#include "addr2str.h"

extern int g_other_port;
extern int g_program_port;


//int g_service_port = 50000;

//*************************************************
int open_socket()
{
    char buff[100];
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) Fatal_Error("Unable to create service socket");

    struct sockaddr_in myaddr;
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(g_program_port);

    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) != 0)
    {
        Fatal_Error("Unable to bind to port %d\n", 0);
    }

    fprintf(stderr, "Bound to %s\n", 
          sock2str(fd, buff)); 

    return fd;
}
//*************************************************
// fd is assumed to be the socket you are communicating over
int get_data(int fd, request_t *request, struct sockaddr_in *remote_addr)
{
    size_t recvlen;
    //request_t request;
    //struct sockaddr_in remote_addr;
    socklen_t addr_len;

    addr_len = sizeof(remote_addr);
    recvlen = recvfrom(fd, request, sizeof(request_t), MSG_TRUNC, 
            (struct sockaddr *)remote_addr, &addr_len);

    if (recvlen < 0) Fatal_Error("Error on service socket");

    if (recvlen == sizeof(request_t))
    {
        return 1;
    } // error
    return 0;
}

//*************************************************
// fd is assumed to be a socket opened for UDP
int send_data(int fd, request_t *request, 
            struct sockaddr_in *remote_addr, int addr_len)
{
    encode(request, request);
    //request->msg_type = htons(request->msg_type);

    ssize_t result = sendto(fd, request, sizeof(request_t), 0, 
            (struct sockaddr *)remote_addr, addr_len);
    if (result != sizeof(request_t))
    {
        // error
    }

    return 0;
}
//*************************************************
void build_server_addr(struct sockaddr_in *myaddr)
{
    struct addrinfo hints;
    struct addrinfo *addr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    if (getaddrinfo("unix.cset.oit.edu", NULL, &hints, &addr) != 0)
    {
        perror("Error getting address info: ");
        Fatal_Error("Error getting address info\n");
    }

    memcpy(myaddr, addr->ai_addr, addr->ai_addrlen);
    myaddr->sin_port = htons(g_other_port);

    freeaddrinfo(addr);
}
//*************************************************
// This code is considered obsolete, but it still works on Loki
void build_server_addr_old(struct sockaddr_in *myaddr)
{
    struct hostent *host;
    if ( (host=gethostbyname2("unix.cset.oit.edu", AF_INET )) == NULL)
    {
        Fatal_Error("Unable to find server's IP address for %s\n", "unix.cset.oit.edu");
    }

    memcpy(&(myaddr->sin_addr.s_addr), host->h_addr_list[0], host->h_length);
    myaddr->sin_family = host->h_addrtype;
    myaddr->sin_port = htons(g_other_port);
}
