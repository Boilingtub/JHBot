#include "Server.h"
#include <openssl/ssl.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
WSADATA wsaData;
#endif

void InitializeOpenSSL() {
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
}

void DestroySSL(struct SSL_Server *ssl_server) {
    //SSL_shutdown(ssl_server->ssl);
    SSL_free(ssl_server->ssl);
    SSL_CTX_free(ssl_server->ctx);
    ERR_free_strings();
    EVP_cleanup();
}

SSL_CTX *create_ssl_context()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    //method = SSLv23_server_method();
    //method = SSLv23_method();
    //method = TLSv1_2_server_method();
    //method = DTLS_method();
    method = TLS_server_method();

    ctx = SSL_CTX_new_ex(NULL,NULL,method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

void configure_context(SSL_CTX *ctx, char* cert_pem)
{
    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, cert_pem, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    printf("SSL certificate set\n");
    if (SSL_CTX_use_PrivateKey_file(ctx, cert_pem, SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    printf("SSL private key set\n");
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
    printf("SSL private key verified");
}

struct SSL_Server SSL_Server_constructor(int domain, int service, int protocol,
                                         unsigned long face, int port, int backlog,
                                         char* cert_pem) {
    struct SSL_Server ssl_server;

    ssl_server.domain = domain;
    ssl_server.service = service;
    ssl_server.protocol = protocol;
    ssl_server.face = face;
    ssl_server.port = port;
    ssl_server.backlog = backlog;

    ssl_server.address.sin_family = domain;
    ssl_server.address.sin_port = htons(port);
    ssl_server.address.sin_addr.s_addr = htonl(face);

     #ifdef _WIN32
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        exit(1);
    }
    #endif

    InitializeOpenSSL();
    
    ssl_server.socket = socket(domain, service, protocol);
   
    #ifdef _WIN32
    if (ssl_server.socket == INVALID_SOCKET) 
    #elif __linux__
    if (ssl_server.socket < 0)
    #endif
    {
        perror("Failed to connect ssl_socket...");
        exit(1);
    }

    //bzero(&ssl_server.address, sizeof(ssl_server.address));
    if(bind(ssl_server.socket, (struct sockaddr*)&ssl_server.address,
            sizeof(ssl_server.address)) < 0) {
        perror("failed to bind ssl_socket...");
        exit(1);
    }

    ssl_server.ctx = create_ssl_context(); 
    
    configure_context(ssl_server.ctx, cert_pem);

    ssl_server.ssl = SSL_new(ssl_server.ctx);

    return ssl_server;
}







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
    
    #ifdef _WIN32
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        exit(1);
    }
    #endif

    server.socket = socket(domain , service , protocol);

    #ifdef _WIN32
    if (server.socket == INVALID_SOCKET) 
    #elif __linux__
    if (server.socket == 0)
    #endif
    {
        perror("Failed to connect socket...\n");
        exit(1);
    }

    if (bind(server.socket , (struct sockaddr *)&server.address, 
        sizeof(server.address)) < 0 )  {
        perror("Failed to bind socket...\n");
        exit(1);
    }

    return server;
} 
