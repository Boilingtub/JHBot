#include "Server.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
WSADATA wsaData;
#endif

struct Server Server_constructor(int domain , int service , int protocol , 
                                 unsigned long face , int port , int backlog) {
    struct Server server;

    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.face = face;
    server.port = port;
    server.backlog = backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(face);

    server.socket = socket(domain , service , protocol);

    #ifdef _WIN32
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        exit(1);
    }
    if (server.socket == INVALID_SOCKET) {
        perror("Failed to connect socket...\n");
        exit(1);
    }
    #elif __linux__
    if (server.socket == 0) {
        perror("Failed to connect socket...\n");
        exit(1);
    }
    #endif

    if (bind(server.socket , (struct sockaddr *)&server.address, 
        sizeof(server.address)) < 0 )  {
        perror("Failed to bind socket...\n");
        exit(1);
    }

    return server;
} 
