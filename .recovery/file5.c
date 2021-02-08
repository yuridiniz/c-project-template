#include "protocol_mqtt.h"
#include "event2/bufferevent.h"

typedef struct mqtt_client {
    char * id;
} mqtt_client_t;

static void send_mqtt(protocol_t * this, client_t * to, char * msg, int len) {
    // TODO Processar dados;
    protocol_t * protocol = this->prev;

    if(protocol) {
        protocol->send(protocol, to, msg, len);
    }
}

static void on_connect_mqtt(protocol_t * this, client_t * client) {
    mqtt_client_t * mqtt_bag = malloc(sizeof(mqtt_client_t));
    mqtt_bag->id = 0;

    set_client_bag(this, client, mqtt_bag);

    protocol_t * protocol = this->next;

    if(protocol) {
        protocol->on_connect(protocol, client);
    }
}

static void onmessage_mqttd(protocol_t * this, client_t * from, char * msg, int len) {
    mqtt_client_t * mqtt_bag = get_client_bag(this, from);

    protocol_t * protocol = this->next;

    if(protocol) {
        protocol->on_message(protocol, from, msg, len);
    }
}

protocol_t * init_mqtt_protocol() {
    protocol_t * protocol = malloc(sizeof(protocol_t));
    protocol->send = &send_mqtt;
    protocol->on_message = &onmessage_mqttd;
    protocol->on_connect = &on_connect_mqtt;
}
