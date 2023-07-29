#include <sys/types.h>
#include <sys/socket.h>
//*************************************************
// Wait for up to timeout seconds for a connection to be made
//
// NOTE: This function will not work correctly if fd 0 (stdin) is closed.
//
// Return values:
//      >0 the fd for the accepted connection
//      =0 timeout expired with no connection
//      <= an error occurred
int timed_accept(int sockfd, 
        struct sockaddr *cli_addr, socklen_t *clilen, 
        int timeout)
{
    int result;
    struct timeval tv;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);

    tv.tv_sec = (long)timeout;
    tv.tv_usec = 0;

    result = select(sockfd+1, &rfds, (fd_set *) 0, (fd_set *) 0, &tv);
    if(result > 0)
    {
        return accept(sockfd, cli_addr, clilen);
    }
    else
    {
        return result;
    }
}

