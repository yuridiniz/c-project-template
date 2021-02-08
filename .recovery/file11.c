#include "protocol_ws.h"
#include "http_parser.h"
#include "base64.h"
#include "sha1.h"


typedef struct ws_client {
    char handshake_done;
} ws_client_t;

static char * generate_accept_key(char * key, size_t * len) {
    const char * magic_string = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    int keyinput_len = strlen(magic_string) + strlen(key);
    char key_input[keyinput_len];
    sprintf(key_input, "%s%s", key, magic_string);

    unsigned char hash[20];
    unsigned char exhash[41];
    sha1digest(hash, exhash, key_input, keyinput_len);

    char * result_key = base64_encode(hash, 20, len);
    base64_cleanup();

    return result_key;
}

static char * generate_accept_response(char * accept_key, size_t accept_key_len, size_t *response_len) {
    const char * fixed_header = "HTTP/1.1 101 Switching Protocols\r\n"
                            "Upgrade: websocket\r\n"
                            "Connection: Upgrade\r\n"
                            "Sec-WebSocket-Accept:";

    char * end_response_token = "\r\n\r\n";
    *response_len = strlen(fixed_header) + accept_key_len + strlen(end_response_token) + 1;
    char * buffer = malloc(sizeof(char) * (*response_len));

    sprintf(buffer, "%s %s%s", fixed_header, accept_key, end_response_token);

    return buffer;
}

static int do_handshake(protocol_t * this, client_t * client, char * msg, int len) 
{
    if(len < 30)
        return -1;
        
    struct Request * req = parse_request(msg);
    struct Header * header = find_header(req, "Upgrade");

    printf("%s\n", msg);

    if(header == NULL || strcmp(header->value, "websocket") != 0) {
        free_request(req);
        return 1;
    }

    header = find_header(req, "Connection");
    if(header == NULL || strcmp(header->value, "Upgrade") != 0) {
        free_request(req);
        return 1;
    }

    header = find_header(req, "Sec-WebSocket-Key");
    if(header == NULL) {
        free_request(req);
        return 1;
    }

    size_t key_size = 0;
    size_t response_size = 0;
    char * accept_key = generate_accept_key(header->value, &key_size);
    char * response = generate_accept_response(accept_key, key_size, &response_size);

    protocol_t * protocol = this->prev;
    protocol->send(protocol, client, response, response_size);

    free(accept_key);
    free(response);
    free_request(req);

    return 0;
}

static void send_ws(protocol_t * this, client_t * to, char * msg, int len) {
    // TODO Processar dados;
    protocol_t * protocol = this->prev;

    if(protocol) {
        protocol->send(protocol, to, msg, len);
    }
}

static void on_connect_ws(protocol_t * this, client_t * client) {
    ws_client_t * bag = malloc(sizeof(ws_client_t));
    bag->handshake_done = 0;

    set_client_bag(this, client, bag);

    protocol_t * protocol = this->next;

    if(protocol) {
        protocol->on_connect(protocol, client);
    }
}

static void onmessage_ws(protocol_t * this, client_t * from, char * msg, int len) {
    ws_client_t * bag = get_client_bag(this, from);
    if(bag->handshake_done == 1) {
        //TODO Implementar processamento da mensagem

        protocol_t * protocol = this->next;

        if(protocol) {
            protocol->on_message(protocol, from, msg, len);
        }
    } else {
        int error = do_handshake(this, from, msg, len);
        if(error == 0) {
            bag->handshake_done = 1;
        }
    }
}


protocol_t * init_ws_protocol() {
    protocol_t * protocol = malloc(sizeof(protocol_t));
    protocol->send = &send_ws;
    protocol->on_message = &onmessage_ws;
    protocol->on_connect = &on_connect_ws;
}