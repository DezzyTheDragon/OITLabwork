#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "nameserver.h"
int open_socket();
int get_data(int fd, request_t *reqest, struct sockaddr_in *remote_addr);
int send_data(int fd, request_t *request, struct sockaddr_in *remote_addr, int addr_len);
void build_server_addr(struct sockaddr_in *myaddr);
void build_server_addr_old(struct sockaddr_in *myaddr);
