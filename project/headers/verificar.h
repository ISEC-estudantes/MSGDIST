#ifndef VERIFICAR_H
#define VERIFICAR_H


/*
 * Trabalho de SO 2019 ISEC
 * 
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 * 
*/
//GESTOR - VIRIFICADOR RELATED FUNCTIONS

#include "utils.h"


//verifica - liga o verificador
//comando-nome do verificador;
//file_proividas-ficheiro com as palavras proibidas;
//rcv-pipe para receber informação do verificador;
//env-pipe para enviar informação ao verificador;
//swicher - 1:liga o verificador, 0:desliga o verificador //ou meter aqui o pid para desligar o verificador
int initverifica ( char *comando, char *file_proibidas, unsigned int *rcv, unsigned int *env, unsigned int *pid );

//verifica uma mensagem msg com o verificador
int verificamsg ( int env, int rcv, char * msg );

//terimina o verificador
void killverifica ( int pid ) ;

//ve os erros que o verificador possa ter
//int errosverificador(char *file_proibidas);

#endif
