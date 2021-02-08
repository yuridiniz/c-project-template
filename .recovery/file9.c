#include "protocol_dummy.h"

static void send_dummy(protocol_t * this, client_t * to, char * msg, int len) {
     // TODO Processar dados;
    protocol_t * protocol = this->prev;

    if(protocol) {
        protocol->send(protocol, to, msg, len);
    }
}

static void on_connect_dummy(protocol_t * this, client_t * client) {
    protocol_t * protocol = this->next;

    if(protocol) {
        protocol->on_connect(protocol, client);
    }
}

static void onmessage_dummy(protocol_t * this, client_t * from, char * msg, int len) {
    server_t * server = from->server;
    client_t ** clients = server->clients;

    int qtd = server->clients_count;

    for(int i = 0; i < qtd; i++) {
        client_t * client = clients[i];
        if(client == from)
            continue;

        protocol_t * protocol = client->server->protocol_stack->top;
        protocol->send(protocol, client, msg, len);
    }

    protocol_t * protocol = this->next;

    if(protocol) {
        protocol->on_message(protocol, from, msg, len);
    }
}

protocol_t * init_dummy_protocol() {
    protocol_t * protocol = malloc(sizeof(protocol_t));
    protocol->send = &send_dummy;
    protocol->on_message = &onmessage_dummy;
    protocol->on_connect = &on_connect_dummy;
}
