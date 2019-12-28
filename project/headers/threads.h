#ifndef THREADS_H
#define THREADS_H

#include "utils.h"
#include <signal.h>

//funcao da primeira thread criada
//  esta função lé o fifo a espera de ordens para enviar info
void readingfifo(void * input);

//adiciona um cliente novo
void addcliente(void* input);

//liberta a memoria do programa
void freethings(global * info);

//checa o nome se é unico
int nomecheck(global *info, char *cliente, cltusr *aux);

//remove um cliente da lista de clientes
void removecliente(void * input);
#endif //THREADS_H
