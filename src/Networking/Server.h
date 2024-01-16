#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include <netinet/in.h>

struct Server {
    int domain;
    int service;
    int protocol;
    unsigned long interface;
    int port;
    int backlog;
    struct sockaddr_in address;
    int socket;
};

struct Server Server_constructor(int domain , int service , int protocol , 
                                 unsigned long interface , int port , int backlog); 
#endif 
