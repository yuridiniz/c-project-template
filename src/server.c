#include "server.h"

typedef struct accept_args {
    server_t * server;
    struct evconnlistener *listener;
    evutil_socket_t fd;
} accept_args_t;

typedef struct read_args {
    client_t * client;
    struct bufferevent *bev;
} read_args_t;


static char 
resolve_protocol(char * msg_reader) {
    return TCP_CONNECTION;
}

void send_broadcast(client_t * sender, char * msg, int len) {
    server_t * server = sender->server;
    client_t ** clients = server->clients;
    int qtd = server->clients_count;

    for(int i = 0; i < qtd; i++) {
        client_t * client = clients[i];
        if(client == sender)
            continue;

        protocol_t * protocol = client->server->protocol_stack->top;
        protocol->send(protocol, client, msg, len);
    }
}

static void read_handler(void * args) 
{
    read_args_t * read = (read_args_t*)(args);
    client_t * client = read->client;

    struct evbuffer * input = bufferevent_get_input(client->buffer);
    int msglen = evbuffer_get_length(input);

    char buffer_reader[msglen];
    bufferevent_read(client->buffer, buffer_reader, msglen);

    protocol_t * protocol = client->server->protocol_stack->bottom;
    protocol->onmessage(protocol, client, buffer_reader, msglen);

    free(args);
}

static void
echo_read_cb(struct bufferevent *bev, void *ctx)
{
    client_t * client = (client_t *) ctx;
    read_args_t * arg = malloc(sizeof(read_args_t));
    arg->client = client;
    arg->bev = bev;

    thpool_add_work(client->server->pool_msg_handler, &read_handler,arg);   
}

static void
echo_event_cb(struct bufferevent *bev, short events, void *ctx)
{
    if (events & BEV_EVENT_ERROR) 
        perror("Error from bufferevent");

    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        bufferevent_free(bev);
        free(ctx);
    } 
    
    if (events & EV_WRITE) {
        client_t * client = (client_t *) ctx;
    }
}


static void accept_conn_handler(void * args) 
{
    accept_args_t * accept = (accept_args_t*)(args);
    server_t * server = accept->server;

    struct event_base *base = evconnlistener_get_base(accept->listener);
    struct bufferevent *bev = bufferevent_socket_new(base, accept->fd, BEV_OPT_CLOSE_ON_FREE);

    client_t * client = malloc(sizeof(client_t));
    client->state = PROTOCOL_UNKNOW;
    client->buffer = bev;
    client->server = server;

    server->clients[server->clients_count++] = client;

    bufferevent_setcb(bev, echo_read_cb, NULL, echo_event_cb, client);

    bufferevent_enable(bev, EV_READ|EV_WRITE);

    free(args);
}


static void
accept_conn_cb(struct evconnlistener *listener,
    evutil_socket_t fd, struct sockaddr *address, int socklen,
    void *ctx)
{
    server_t * server = (server_t*)(ctx);

    accept_args_t * arg = malloc(sizeof(accept_args_t));
    arg->server = server;
    arg->listener = listener;
    arg->fd = fd;

    thpool_add_work(server->pool_accept, &accept_conn_handler, arg);
}

static void
accept_error_cb(struct evconnlistener *listener, void *ctx)
{
    struct event_base *base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Got an error %d (%s) on the listener. "
            "Shutting down.\n", err, evutil_socket_error_to_string(err));

    event_base_loopexit(base, NULL);
}



server_t * init_server(uint max_client, uint pool_accepts, uint pool_msg) {
    server_t * server = malloc(sizeof(server_t));
    server->clients_count = 0;
    server->max_client = max_client;
    server->pool_accept = thpool_init(pool_accepts);
    server->pool_msg_handler = thpool_init(pool_msg);

    return server;
}

int start_listern(server_t* server) {
    struct event_base *base;
    struct sockaddr_in sin;
    struct evconnlistener *listener;

    memset(&sin, 0, sizeof(sin));
    base = event_base_new();

    server->clients_count = 0;
    server->clients = malloc(server->max_client * sizeof(client_t*));

    sin.sin_family = AF_INET;
    sin.sin_port = htons(server->port);

    inet_aton(server->host, &sin.sin_addr);

    listener = evconnlistener_new_bind(base, accept_conn_cb, server,
            LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
            (struct sockaddr*)&sin, sizeof(sin));

    if (!listener) {
        perror("Couldn't create listener");
        return 1;
    }

    evconnlistener_set_error_cb(listener, accept_error_cb);

    event_base_dispatch(base);

    return 0;
}
