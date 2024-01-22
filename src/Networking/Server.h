#ifndef Server_h
#define Server_h

#ifdef _WIN32
#include <winsock2.h>
#elif __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#endif

struct Server {
    int domain;
    int service;
    int protocol;
    unsigned long face;
    int port;
    int backlog;
    struct sockaddr_in address;
    int socket;
};

struct Server Server_constructor(int domain , int service , int protocol , 
                                 unsigned long face , int port , int backlog); 
#endif 
