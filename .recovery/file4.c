#include "protocol_tcp.h"

static void send_tcp(protocol_t * this, client_t * to, char * msg, int len) {
    if(to->buffer) {
        bufferevent_write(to->buffer, msg, len);
    }

     // TODO Processar dados;
    protocol_t * protocol = this->prev;

    if(protocol) {
        protocol->send(protocol, to, msg, len);
    }
}

static void on_connect_tcp(protocol_t * this, client_t * client) {
    protocol_t * protocol = this->next;

    if(protocol) {
        protocol->on_connect(protocol, client);
    }
}

static void onmessage_tcp(protocol_t * this, client_t * from, char * msg, int len) {
    protocol_t * protocol = this->next;

    if(protocol) {
        protocol->on_message(protocol, from, msg, len);
    }
}

protocol_t * init_tcp_protocol() {
    protocol_t * protocol = malloc(sizeof(protocol_t));
    protocol->send = &send_tcp;
    protocol->on_message = &onmessage_tcp;
    protocol->on_connect = &on_connect_tcp;
}
