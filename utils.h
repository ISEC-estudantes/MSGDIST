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
#include<fcntl.h>

#include "cliente.h"
#include "mensagens.h"

#pragma once
//defenicoes de default

//nome do file com as mensagens proibidas
#define DEF_WORDSNOT "proibidas"

//numero de maximo de palavras proibidas antes de se nao se aceitar a mensagem
#define DEF_MAXNOT 3

//numero maximo de mensagens
#define DEF_MAXMSG 100

//timeout de default das mensagens
#define DEF_MAXTIMEOUT 60

//maximo de users que podem estar ligados ao gestor
#define DEF_MAXUSERS 10


//vai buscar as variaveis de ambiente necessarias
//se nao existirem vai substituir com os macros apresentados acima
int getvars(int *maxmsg, int *maxnot, char * wordsnot, int *maxtimeout, int *maxusers);


//funcao para ir buscar opcoes dos argumentos
int getoption(int argc, char **argv, int *filter, int *cmd, int * help, int * debug);

//insere uma mao imoji a apontar para o lado como um > ou $ no inicio de um comando
void insere();

#endif
