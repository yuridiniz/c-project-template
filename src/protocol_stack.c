#include "protocol_stack.h"

static void send_stack(protocol_stack_t * this, client_t * to, char * msg, int len) {
    // TODO Processar dados;
    protocol_t * bottom = this->bottom;

    if(bottom) {
        bottom->send(bottom, to, msg, len);
    }
}

static void onmessage_stack(protocol_stack_t * this, client_t * from, char * msg, int len) {
    protocol_t * top = this->top;

    if(top) {
        top->onmessage(top, from, msg, len);
    }
}

protocol_stack_t * init_protocol_stack(protocol_t ** itens, int qtd) 
{

    protocol_stack_t * stack = malloc(sizeof(protocol_stack_t));

    stack->bottom = itens[0];
    stack->top = itens[qtd - 1];

    for(int i = 0; i < qtd; i++) {
        if(i + 1 < qtd)
            itens[i]->next = itens[i + 1];
        
        if(i > 0)
            itens[i]->prev = itens[i - 1];
    }

    return stack;
}
