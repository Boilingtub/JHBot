#ifndef Server_h
#define Server_h

#ifdef _WIN32
#include <winsock2.h>
#elif __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/cryptoerr_legacy.h>

struct SSL_Server {
    int domain;
    int service;
    int protocol;
    unsigned long face;
    int port;
    int backlog;

    struct sockaddr_in address;
    int socket;

    int use_certificate;
    int use_privatekey;

    SSL_CTX *ctx;
    SSL *ssl;
};

void InitializeOpenSSL();

void DestroySSL(struct SSL_Server *ssl_server); 

struct SSL_Server SSL_Server_constructor(int domain, int service, int protocol,
                                         unsigned long face, int port, int backlog,
                                         char* cert_pem);

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
