#ifndef UTILS_H
#define UTILS_H

/*
 * Trabalho de SO 2019 ISEC
 * 
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 * 
*/
//ALL - GENERIC INFORMATION

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <getopt.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>

//necessitam de bibliotecas extras
#include <pthread.h> //adicionar no compilador -lpthread
#include <ncurses.h> //adicionar no compilador -lncurses


///strutura global
typedef struct _global global;

//estrutura para adicao de um novo cliente
typedef struct _pipemsg pipemsg;

#include "cltusr.h"
#include "mensagens.h"





struct _global {
    //locker do que esta no info
    pthread_mutex_t lock_info;
    
    //sobre as listas e estruturas
    unsigned int nclientes, ntopicos;
    
    cltusr *listclientes , *lastclient;

    pthread_mutex_t lock_cltusr;
    
    tpc *listtopicos, *lasttopic;
    pthread_mutex_t lock_tpc;

    //variaveis globais
    unsigned int debug, maxmsg, maxnot, maxtimeout, maxusers, filter;
    char *wordsnot;

    //pid do verificador
    unsigned int cpid;

    //info do verificador
    unsigned int rcv, env;

    //variavel para terminar os processos
    int terminate;

    //thread handlears
    pthread_t read_fifo;
    pthread_t timer;
    
    //valores temporariso para usar
    int tempint;//um valor int
    void *temppointer; //um ponteiro para usar
};




//16 bytes de ints + 1150 bytes de chars = 1k164 bytes
struct _pipemsg {

    //codigo id
    int codigo;
    
    //id do topico
    int topicid;
    char topicname[50];

    //id da mensagem
    int msgid;
    //titulo da mensagem
    char  titulo[50];
    //corpo da mensagem
    char corpo[1000];

    //cliente que a emviou
    pid_t pid;
    char clientname[50];
};




//defenicoes de codigo de default
//nome do fifo do gestor
#define GESTORFIFO "gestor-fifo"


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




                        //DEFENITIONS OF CODES



//quando as defenicoes de criacao do cliente sao invalidas(provavelmente por causa do nome)
#define INVALID_CLIENT_NAME -1

//quando se atingui o numero maximo de clientes
#define INVALID_CLIENT_MAX -2

//tudo correu como esperado
#define OK_SENT 0

//para adicionar um cliente
#define ADD_CLIENT 1

//para enviar uma mensagem
#define ADD_MESSAGE 2

//para enviar uma subrescricao
#define SUBSCRITION 3

//defined to say that the system is shuting down
#define SHUTDOWN -100

typedef struct _sub sub;

struct _sub {
    int pid;

};

#define CLOSING_CLIENT 4


//vai buscar as variaveis de ambiente necessarias
//se nao existirem vai substituir com os macros apresentados acima
int getvars(int *maxmsg, int *maxnot, char *wordsnot, int *maxtimeout, int *maxusers);

//funcao para ir buscar opcoes dos argumentos
int getoption(int argc, char **argv, int *filter, int *cmd, int *help, int *debug);

//insere uma mao imoji a apontar para o lado como um > ou $ no inicio de um comando
void insere();

void semmem();

//inicializa o info com valores neutros
global * initinfo();

//initializa todos os valores a zero excepto os arrays de char
pipemsg initpipemsg();

#endif
