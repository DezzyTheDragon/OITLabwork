#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <signal.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include "timed_accept.h"
#include "getport.h"
#include "addr2str.h"
#include "readline.h"

int static g_server_running = 1;

typedef struct {
    int maxThreads;
    int backlogSize;
    char serviceName[10];
    char hostName[20];
    int nameserverPort;
} t_serverInfo;

static void sig_handler(int signal)
{
    fprintf(stderr, "Recived broken pipe signal\n");
}

int getPortNumber(t_serverInfo info)
{
    //if(setup_ns("unix.cset.oit.edu", 50000))
    if(setup_ns(info.hostName, info.nameserverPort))
    {
        printf("Failed to connect to nameserver\n");
        exit(-1);
    }
    uint32_t timeout;
    //int port = request_port("DD_Lab3", &timout);
    int port = request_port(info.serviceName, &timeout);
    if(port < 0)
    {
        //I might already have a service in the name server
        printf("Unable to reaquest new port\n");
        //port = lookup_port("DD_Lab3");
        port = lookup_port(info.serviceName);
        if(port < 0)
        {
            //something very very wrong happend
            printf("Error occored when getting port\n");
            exit(-1);
        }
        else
        {
            //keep_alive_port("DD_Lab3", port);
            keep_alive_port(info.serviceName, port);
        }
    }
    return port;
}

int newSock(int port)
{
    int FD = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in myAddr;
    memset((char*)&myAddr, 0, sizeof(myAddr));
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myAddr.sin_port = htons(0);
    if(bind(FD, (struct sockaddr *)&myAddr, sizeof(myAddr)) != 0)
    {
        printf("Unable to bind to port\n");
        exit(-1);
    }
    if(listen(FD, 5) != 0)
    {
        printf("Unable to listen to port\n");
        exit(-1);
    }
    return FD;
}

void fileTransfer(char *filename, int tcpFD)
{
    int fileFD;
    char buff[256];
    memset(buff, 0, sizeof(buff));
    int size;
    Line_Reader_t fileLR = readline_open(tcpFD);

    //open file
    fileFD = open(filename, O_RDONLY);
    if(fileFD < 0)
    {
        printf("Unable to open %s", filename);
        return;
    }

    size = read(fileFD, buff, sizeof(buff));
    while(size > 0)
    {
        printf(buff);
        //send
        writestr(buff, fileLR);
        //read next bit
        size = read(fileFD, buff, sizeof(buff));
    }

    close(fileFD);
    readline_close(fileLR);
    //calc and send MD5

}

//Function that handles the client
void *handleClient(void *ptr)
{
    //convert the argument into a file descriptor
    int fd = *(int *)ptr;
    //this is a thread function so detatch itself
    pthread_detach(pthread_self());

    //setup data
    char buff[100];
    char *connected = buff;
    int running = 1;
    Line_Reader_t lr = readline_open(fd);
    //interaction loop, keep looping untill we are done
    //with the server
    while(connected != NULL && running)
    {
        //Waiting for user input
        printf("Client %d waiting for input\n", fd);
        connected = readline(buff, sizeof(buff), lr);
        //check to maks sure that the sent data isn't null
        if (connected != NULL)
        {
            printf("Recived: %s", buff);
            char *command;
            //tokenize the input, get the first word
            command = strtok(buff, " \n");
            
            //To ensure that a blank command dosnt crash server
            if(command != NULL && 
                    strcmp(command, "get") == 0)
            {
                //Get command takes a directory and sends all the files
                //in that directory
                printf("Recived 'get' command\n");
                
                //get the directory from the command
                command = strtok(NULL, " \n");
                if(command != NULL)
                {
                    //directory given
                    
                    DIR *dirp;
                    struct dirent *dp;

                    if((dirp = opendir(command)) == NULL)
                    {
                        char *errorMsg;
                        asprintf(&errorMsg, "error: failed to open directory %s", command);
                        writestr(errorMsg, lr);
                        free(errorMsg);
                    }
                    else
                    {
                        do
                        {
                            errno = 0;
                            if((dp = readdir(dirp)) != NULL)
                            {
                                if(strstr(dp->d_name, ".txt") != NULL)
                                {
                                    //send the file here
                                    
                                    //format file message
                                    char *fileMsg;
                                    int fileFD = newSock(0);
                                    
                                    asprintf(&fileMsg, "file: %d %s\n",
                                            sock2port(fileFD), dp->d_name);
                                    printf("Client %d sending:\n\t%s", fd, fileMsg);
                                    writestr(fileMsg, lr);
                                    //Send file
                                    int fileConFD = accept(fileFD, NULL, NULL);
                                    fileTransfer(dp->d_name, fileConFD);
                                    
                                    //Send MD5
                                    char *md5Com;
                                    asprintf(&md5Com, "md5sum %s/%s", command, dp->d_name);
                                    FILE *md5R = popen(md5Com, "r");
                                    char sum[32];
                                    fgets(sum, sizeof(sum), md5R);
                                    char *md5;
                                    asprintf(&md5, "md5sum: %s\n", sum);
                                    printf(md5);
                                    writestr(md5, lr);
                                    free(md5);
                                    free(md5Com);                                    

                                    //clean up
                                    pclose(md5R);
                                    free(fileMsg);
                                    close(fileConFD);
                                    close(fileFD);

                                }
                            }
                        }while(dp != NULL);
                        writestr("done\n", lr);
                    }

                    //Remember to sanatize file inputs and throw errors for
                    //bad names
                    //for example filenames like <file"&&"ls -la.txt">

                    /* 
                    //create and bind to a socket
                    int fileFD = newSock(0);
                    char fileMsgBuff[100];
                    printf("Client %d File Transfer on %s\n", 
                        fd, sock2str(fileFD, fileMsgBuff));
                    //Listen to the port
                    if(listen(fileFD, 5) != 0)
                    {
                        printf("Unable to listen on file transfer\n");
                        exit(-1);
                    }
                    //tell the client what to expect
                    char *portMsg;
                    asprintf(&portMsg, "file: %d file1.txt\n", sock2port(fileFD));
                    writestr(portMsg, lr);
                    free(portMsg);
                    //connect and send the file
                    int fileConFD = accept(fileFD, NULL, NULL);
                    close(fileConFD);
                
                    //File 2 same song second verse
                    int file2FD = newSock(0);
                    asprintf(&portMsg, "file: %d file2.txt\n", sock2port(file2FD));
                    memset(fileMsgBuff, 0, sizeof(fileMsgBuff));
                    printf("Client %d File Transfer on %s\n",
                        fd, sock2str(file2FD, fileMsgBuff));
                    if(listen(file2FD, 5) != 0)
                    {
                        printf("Unable to listen on file 2 transfer\n");
                        exit(-1);
                    }
                    writestr(portMsg, lr);
                    free(portMsg);
                    int file2ConFD = accept(file2FD, NULL, NULL);
                    close(file2ConFD);
                    */

                    //writestr("done\n", lr);
                }
                else
                {
                    writestr("error: no directory given\n", lr);
                }
            }
            else if(command != NULL &&
                    strcmp(command, "exit") == 0)
            {
                printf("Recived 'exit' command\n");
                running = 0;
                writestr("exit\n", lr);
            }
            else if(command != NULL &&
                    strcmp(command, "shutdown") == 0)
            {
                printf("Recived 'shutdown' command\n");
                writestr("shutdown\n", lr);
                g_server_running = 0;
                running = 0;
            }
            else
            {
                printf("Recived unknown '%s' command\n", command);
                writestr("ERROR\n", lr);
            }
            

            /*
               printf("Sending back: %s\n", msg);
               writestr(msg, lr);
               */
            
        }
    }
    printf("Done talking to client %d\n", fd);
    readline_close(lr);
    close(fd);
    return NULL;
}

void parseArg(int argc, char** argv, t_serverInfo *info)
{
    int opt;
    while((opt = getopt(argc, argv, "c:hl:n:N:p:")) != -1)
    {
        switch(opt)
        {
            case 'c':
                //printf("Thread command\n");
                info->maxThreads = atoi(optarg);
                break;
            case 'h':
                printf("Help\n-c <num>: Max thread count\n\
                        -h: help and exit\n\
                        -l <num>: set the listening backlog size\n\
                        -n <service name>: set the service name\n\
                        -N <addr>: The address of the nameserver\n\
                        -p <port>: The port for the nameserver\n");
                exit(0);
                break;
            case 'l':
                //printf("Listing backlog command\n");
                info->backlogSize = atoi(optarg);
                break;
            case 'n':
                //printf("Service name command\n");
                memcpy(info->serviceName, optarg, sizeof(info->serviceName));
                break;
            case 'N':
                //printf("Host command\n");
                memcpy(info->hostName, optarg, sizeof(info->serviceName));
                break;
            case 'p':
                //printf("Port command\n");
                info->nameserverPort = atoi(optarg);
                break;
        }
    }
}

int main(int argc, char **argv)
{
    //Request port from nameserver
    //Use that port to listen to TCP requests
    //Spin up a new thread to handle the request

    //Client can send
    //get <directory>
    //exit
    //shutdown
    //all '\n' terminated

    //Start with basic connection and send back just errors

    if(signal(SIGPIPE, sig_handler) == SIG_ERR)
    {
        fprintf(stderr, "Unable to set up signal handler\n");
    }

    t_serverInfo serverInfo;
    serverInfo.maxThreads = 2;
    serverInfo.backlogSize = 5;
    memset(serverInfo.serviceName, 0, sizeof(serverInfo.serviceName));
    strcpy(serverInfo.serviceName, "DD_Lab3");
    memset(serverInfo.hostName, 0, sizeof(serverInfo.hostName));
    strcpy(serverInfo.hostName, "unix.cset.oit.edu");
    serverInfo.nameserverPort = 50000;

    parseArg(argc, argv, &serverInfo);

    int listenPort = getPortNumber(serverInfo);

    printf("Port to use is %i\n", listenPort);
    
    //socket
    //bind
    //listen
    //loop
    //accept
    
    //Buffer for printing
    char buff[100];
    //Start socket
    int listenFD = socket(AF_INET, SOCK_STREAM, 0);
    //Formulate my address
    struct sockaddr_in myaddr;
    memset((char*)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(listenPort);
    //Bind to socket
    if(bind(listenFD, (struct sockaddr *)&myaddr, sizeof(myaddr)) != 0)
    {
        printf("Unable to bind to port %i\n", listenPort);
        exit(-1);
    }
    printf("Bound to %s\n", sock2str(listenFD, buff));

    //Listen
    if(listen(listenFD, serverInfo.backlogSize) != 0)
    {
        printf("Unable to listen to port\n");
        exit(-1);
    }
    //while(g_server_running)
    do
    {
        //formulate address of the sender
        struct sockaddr peerAddr;
        memset(&peerAddr, 0, sizeof(struct sockaddr));
        socklen_t addrLen;
        memset(&addrLen, 0, sizeof(socklen_t));
        //accept TCP connection
        //int clientFD = accept(listenFD, &peerAddr, &addrLen);
        int clientFD = timed_accept(listenFD, &peerAddr, &addrLen, 5);
        keep_alive_port(serverInfo.serviceName, listenPort);
        if(clientFD > 0)
        {
            printf("Client connected. Using fd %d\n", clientFD);
            //handleClient(clientFD);
            pthread_t thread;
            pthread_create(&thread, NULL, handleClient, (void *)&clientFD);
            //pthread_detach(thread);
        }
    }
    while(g_server_running);

    //cleanup
    //release_port("DD_Lab3", listenPort);
    release_port(serverInfo.serviceName, listenPort);
    return 0;
}
