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
    struct bufferevent *buffer;
    server_t *server;
    void ** bag;
    ushort bag_size;
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
    int id;
    protocol_t * next;
    protocol_t * prev;
    void (*send)(protocol_t * this, client_t * to, char * data, int len);
    void (*on_message)(protocol_t * this, client_t * from, char * data, int len);
    void (*on_connect)(protocol_t * this, client_t * client);
} protocol_t;

typedef struct protocol_stack {
    int size;
    protocol_t * top;
    protocol_t * bottom;
} protocol_stack_t;

server_t * init_server(uint max_client, uint pool_accepts, uint pool_msg);
int start_listern(server_t*);
void * get_client_bag(protocol_t * protocol, client_t * client);
void set_client_bag(protocol_t * protocol, client_t * client, void* bag);

#endif