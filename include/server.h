#ifndef SERVER_H
#define SERVER_H

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <arpa/inet.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "thpool.h"

#define PROTOCOL_UNKNOW 0b00000000
#define TCP_CONNECTION 0b00000001
#define WS_HANDSHAKE_PENDING 0b00000010
#define WS_CONNECTION 0b00000100

typedef struct server server_t;
typedef struct protocol_stack protocol_stack_t;
typedef struct protocol protocol_t;

typedef struct client {
    char state;
    char * id;
    struct bufferevent *buffer;
    server_t *server;
} client_t;

typedef struct server {
    uint max_client;
    uint clients_count;
    char * host;
    ushort port;
    client_t ** clients;
    threadpool pool_accept; 
    threadpool pool_msg_handler;
    protocol_stack_t * protocol_stack;

} server_t;

typedef struct protocol {
    protocol_t * next;
    protocol_t * prev;
    void (*send)(protocol_t * this, client_t * to, char * data, int len);
    void (*onmessage)(protocol_t * this, client_t * from, char * data, int len);
} protocol_t;

typedef struct protocol_stack {
    protocol_t * top;
    protocol_t * bottom;
} protocol_stack_t;

server_t * init_server(uint max_client, uint pool_accepts, uint pool_msg);
int start_listern(server_t*);


#endif