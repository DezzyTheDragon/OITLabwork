#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include "getport.h"
#include "readline.h"
#include "addr2str.h"

typedef struct{
    char serviceName[50];
    char nameserver[50];
    int nameserverPort;
} t_connectionInfo;

int getPortNumber(t_connectionInfo info)
{
    //if(setup_ns("unix.cset.oit.edu", 50000))
    if(setup_ns(info.nameserver, info.nameserverPort))
    {
        printf("Failed to connect to nameserver\n");
        exit(-1);
    }
    //int port = lookup_port("DD_Lab3");
    int port = lookup_port(info.serviceName);
    if(port < 0)
    {
        printf("Error occored when finding service\n");
        exit(-1);
    }
    return port;
}

void writeFile(char *fileName, int port)
{
    //open file connection
    int fileSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in myaddr;
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(0);
    if(bind(fileSocketFD, (struct sockaddr*)&myaddr, sizeof(myaddr)) != 0)
    {
        printf("Failed to bind to port for file transfer\n");
        exit(-1);
    }

    printf("Bound to file port %d\n", sock2port(fileSocketFD));
    
    struct addrinfo hints;
    struct addrinfo *address;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    if(getaddrinfo("unix.cset.oit.edu", NULL, &hints, &address))
    {
        printf("Unable to get server address\n");
        exit(-1);
    }
    struct sockaddr_in serverAddress;
    memcpy(&serverAddress, address->ai_addr, address->ai_addrlen);
    serverAddress.sin_port = htons(port);
    if(connect(fileSocketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0)
    {
        printf("Unable to connect for file transfer\n");
        exit(-1);
    }
    printf("Reciveing file %s\n", fileName);
    Line_Reader_t fileLR; 
    fileLR = readline_open(fileSocketFD);
    char buff[256];
    char *con = buff;
    con = readline(buff, sizeof(buff), fileLR);
    //open file
    char *clientFileName;
    asprintf(&clientFileName, "dump/%s", fileName);
    int writeFD = open(clientFileName, O_APPEND | O_CREAT | O_RDWR, 0777);
    if(writeFD < 0)
    {
        printf("Unable to open or create file %s\n", clientFileName);
        exit(-1);
    }
    free(clientFileName);
    while(con != NULL)
    {
        printf(buff);
        //write to file
        //write(writeFD, buff, sizeof(buff));
        con = readline(buff, sizeof(buff), fileLR);
    }
    //close file
    close(writeFD);
    printf("Finished reciving file %s\n", fileName);
    readline_close(fileLR);
    close(fileSocketFD);
}

void commandTest(int fd)
{
    Line_Reader_t lr = readline_open(fd);
    char msg[50];
    char buff[100];
    char *connected = buff;
    int running = 1;
    while(connected != NULL && running)
    {
        printf("Input command: ");
        fgets(msg, 50, stdin);
        printf(msg);
        writestr(msg, lr);
        connected = readline(buff, sizeof(buff), lr);
        if(connected != NULL)
        {
            printf("Recived: %s", buff);
            char *recived;
            recived = strtok(buff, " \n");
            printf("\tDEBUG_TOKEN: %s\n", recived);
            if(strcmp(recived, "exit") == 0 
                    || strcmp(recived, "shutdown") == 0)
            {
                running = 0;
            }
            else if(strcmp(recived, "file:") == 0)
            {
                //get port number
                //connect to port
                while(strcmp(recived, "done") != 0)
                {
                    recived = strtok(NULL, " \n");
                    int filePort = atoi(recived);
                    char fileName[50];
                    recived = strtok(NULL, " \n");
                    memcpy(fileName, recived, sizeof(fileName));
                    printf("Getting file %s from server on port %d\n", 
                        fileName, filePort);
                    writeFile(fileName, filePort);

                    connected = readline(buff, sizeof(buff), lr);
                    recived = strtok(connected, " \n");
                    if(strcmp(recived, "md5sum:") != 0)
                    {
                        printf("No md5sum was recived for %s\n", fileName);
                    }
                    else
                    {
                        //get md5sum from server
                        recived = strtok(NULL, " \n");
                        printf("md5sum recived is %s\n", recived);
                        //calculate md5sum for what was recived
                        char* md5Com;
                        asprintf(&md5Com, "md5sum dump/%s", fileName);
                        FILE *md5R = popen(md5Com, "r");
                        char sum[32];
                        fgets(sum, sizeof(sum), md5R);
                        char *md5;
                        asprintf(&md5, "md5sum calculated is %s\n", sum);
                        printf(md5);
                        if(strcmp(recived, sum) != 0)
                        {
                            printf("\tERROR: MD5SUM_MISMATCH\n");
                        }
                        free(md5Com);
                        free(md5);
                        pclose(md5R);
                        //Next server message
                        connected = readline(buff, sizeof(buff), lr);
                        recived = strtok(connected, " \n");
                    }
                //Connect to the server
                /*
                int fileSocketFD = socket(AF_INET, SOCK_STREAM, 0);
                struct sockaddr_in myaddr;
                memset((char*)&myaddr, 0, sizeof(myaddr));
                myaddr.sin_family = AF_INET;
                myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
                myaddr.sin_port = htons(0);
                if(bind(fileSocketFD, (struct sockaddr *)&myaddr,
                            sizeof(myaddr)) != 0)
                {
                    printf("Failed to bind to file port\n");
                    exit(-1);
                }
                char fileMsgBuff[100];
                printf("Bound to file port: %s\n", 
                        sock2str(fileSocketFD, fileMsgBuff));
                struct addrinfo hints;
                struct addrinfo *address;
                memset(&hints, 0, sizeof(hints));
                hints.ai_family = AF_INET;
                if(getaddrinfo("unix.cset.oit.edu", NULL, &hints, &address) != 0)
                {
                    printf("Unable to get Loki address\n");
                    exit(-1);
                }
                struct sockaddr_in serverAddress;
                memcpy(&serverAddress, address->ai_addr, address->ai_addrlen);
                serverAddress.sin_port = htons(filePort);
                if(connect(fileSocketFD, (struct sockaddr *)&serverAddress,
                            sizeof(serverAddress)) != 0)
                {
                    printf("Unable to connect to server\n");
                    exit(-1);
                }
                
                connected = readline(buff, sizeof(buff), lr);
                
                recived = strtok(connected, " \n");
                
                close(fileSocketFD);
                */
                }
            }
        }
    }
    
    readline_close(lr);
}

void comunicate(int fd)
{

    Line_Reader_t lr = readline_open(fd);
    char msg[25];
    strcpy(msg, "Hi from client\n");
    char readBuff[100];
    printf("Sending %s\n", msg);
    writestr(msg, lr);
    readline(readBuff, sizeof(readBuff), lr);
    printf("Recived %s\n", readBuff);
    readline_close(lr);
}

void sig_pipeFail(int fd)
{
    Line_Reader_t lr = readline_open(fd);
    char msg[25];
    strcpy(msg, "Die pipe!\n");
    printf("Attempting to kill pipe\n");
    writestr(msg, lr);
    close(fd);
    readline_close(lr);
}

void parseArg(int argc, char** argv, t_connectionInfo *info)
{
    int opt;
    while((opt = getopt(argc, argv, "hn:N:p:")) != -1)
    {
        switch(opt)
        {
            case 'h':
                printf("Help\n-n <service name>: service name of the file server\
                        -N <nameserver>: address of the nameserver\
                        -p <port>: the port number of the nameserver\n");
                break;
            case 'n':
                memset(info->serviceName, 0, sizeof(info->serviceName));
                memcpy(info->serviceName, optarg, sizeof(info->serviceName));
                break;
            case 'N':
                memset(info->nameserver, 0, sizeof(info->nameserver));
                memcpy(info->nameserver, optarg, sizeof(info->nameserver));
                break;
            case 'p':
                info->nameserverPort = atoi(optarg);
                break;
        }
    }
}

int main(int argc, char **argv)
{
    t_connectionInfo con_info;
    memset(con_info.serviceName, 0, sizeof(con_info.serviceName));
    strcpy(con_info.serviceName, "DD_Lab3");
    memset(con_info.nameserver, 0, sizeof(con_info.nameserver));
    strcpy(con_info.nameserver, "unix.cset.oit.edu");
    con_info.nameserverPort = 50000;
    parseArg(argc, argv, &con_info);

    //create socket
    int connectFD = socket(AF_INET, SOCK_STREAM, 0);
    //set info for binding
    struct sockaddr_in myaddr;
    memset((char*)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(0);
    //bind to socket
    if(bind(connectFD, (struct sockaddr *)&myaddr, sizeof(myaddr)) != 0)
    {
        printf("Unable to bind to port\n");
        exit(-1);
    }
    char buff[100];
    printf("Bound to %s\n", sock2str(connectFD, buff));
    //formulate address to connect to
    int serverPort = getPortNumber(con_info);
    struct addrinfo hints;
    struct addrinfo *address;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    //if(getaddrinfo("unix.cset.oit.edu", NULL, &hints, &address) != 0)
    if(getaddrinfo(con_info.nameserver, NULL, &hints, &address) != 0)
    {
        printf("Error getting address info\n");
        exit(-1);
    }
    struct sockaddr_in serverAddress;
    //memset(serverAddress, 0, sizeof(struct sockaddr_in));
    memcpy(&serverAddress, address->ai_addr, address->ai_addrlen);
    serverAddress.sin_port = htons(serverPort);
    
    //struct socklen_t sockLen;
    //memset(&sockLen, 0, sizeof(sockLen));
    //Connect to server
    if(connect(connectFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0)
    {
        printf("Failed to connect to server\n");
        exit(-1);
    }
    //Try to send and recive data
    commandTest(connectFD);
    //sig_pipeFail(connectFD);

    close(connectFD);
    
    
    //freeaddrinfo(serverAddress);
    return 0;
}
