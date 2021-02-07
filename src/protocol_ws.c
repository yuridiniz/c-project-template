#include "protocol_ws.h"

static void send_ws(protocol_t * this, client_t * to, char * msg, int len) {
    // TODO Processar dados;
    protocol_t * protocol = this->prev;

    if(protocol) {
        protocol->send(protocol, to, msg, len);
    }
}

static void onmessage_ws(protocol_t * this, client_t * from, char * msg, int len) {
    // TODO Processar dados;
    protocol_t * protocol = this->next;

    if(protocol) {
        protocol->onmessage(protocol, from, msg, len);
    }
}

protocol_t * init_ws_protocol() {
    protocol_t * protocol = malloc(sizeof(protocol_t));
    protocol->send = &send_ws;
    protocol->onmessage = &onmessage_ws;
}