#pragma once

//Creates a new TCP socket, binds, and listens on the socket
//Takes the port number for the socket
//Returns a file descriptor
int newSocket(int port);

