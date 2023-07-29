#include <sys/types.h>
#include <sys/socket.h>
int timed_accept(int sockfd, struct sockaddr *cli_addr, 
        socklen_t *clilen, int timeout);
