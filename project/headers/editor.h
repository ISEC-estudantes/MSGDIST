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

//menu para ver os topicos
void vertopicos(global * info);

//printar uma nova notificacao
//  se o nlinhas for defenido ele vai criar as cenas, 
//  e por default deve se meter um 0 no nlinhas e ncolunas
void newnot(global *info, char *msg, int nlinhas, int ncolunas);

msg msgui(global *info, int tpcid);


#endif //EDITOR_H


