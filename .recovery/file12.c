#include "protocol_stack.h"

protocol_stack_t * init_protocol_stack(protocol_t ** itens, int qtd) 
{
    protocol_stack_t * stack = malloc(sizeof(protocol_stack_t));

    stack->bottom = itens[0];
    stack->top = itens[qtd - 1];
    stack->size = qtd;

    for(int i = 0; i < qtd; i++) {
        itens[i]->id = i;

        if(i + 1 < qtd)
            itens[i]->next = itens[i + 1];
        
        if(i > 0)
            itens[i]->prev = itens[i - 1];
    }

    return stack;
}
