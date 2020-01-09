#ifndef THREADS_H
#define THREADS_H

/*
 * Trabalho de SO 2019 ISEC
 * 
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 * 
*/
//GESTOR - FIFO READER THREAD(S)

#include "utils.h"
#include <signal.h>

//funcao da primeira thread criada
//  esta função lé o fifo a espera de ordens para enviar info
void readingfifo(void * input);

//adiciona um cliente novo
void addcliente(global* info, pipemsg * recebe);

//liberta a memoria do programa
void freethings(global * info);

//checa o nome se é unico, se nao for retorna o null, 
//por default returna um nome unica diacordo com o que ja existe
//na lista de clientes
char * nomecheck(global *info, char *cliente, int pid);

//apaga as mensagens quando chega a sua hora de bater as botas
void timerdeleter(void *input);

#endif //THREADS_H
