#ifndef EDITOR_H
#define EDITOR_H

/*
 * Trabalho de SO 2019 ISEC
 * 
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 * 
*/
//CLIENTE - MSG EDITOR THREAD(S)

#include "utils.h"

//base do ui para o cliente
void ui(void *input);

//opcoes para a mensagem
void escrevermsg(global *info);

//adiciona um topico
void addtpcui(void * input);

#endif //EDITOR_H


