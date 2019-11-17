#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <getopt.h>
#include <getopt.h>
#include <ctype.h>
#include "cliente.h"

//defenicoes de default

//nome do file com as mensagens proibidas
#define DEF_WORDSNOT "proibidas"

//numero de maximo de palavras proibidas antes de se nao se aceitar a mensagem
#define DEF_MAXNOT 3

//numero maximo de mensagens
#define DEF_MAXMSG 100


//vai buscar as variaveis de ambiente necessarias
//se nao existirem vai substituir com os macros apresentados acima
int getvars(int *maxmsg, int *maxnot, char * wordsnot);


//funcao para ir buscar opcoes dos argumentos
int getoption(int argc, char **argv, int *filter, int *cmd, int * help, int * debug);


#endif
