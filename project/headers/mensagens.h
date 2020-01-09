#ifndef MENSAGENS_H
#define MENSAGENS_H

#include "utils.h"

/*
 * Trabalho de SO 2019 ISEC
 * 
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 * 
*/
//ALL - GENERIC TPC AND MSG MANAGEMENT

//variaveis de contagem

typedef struct _tpc tpc;
typedef struct _msg msg;

struct _tpc
{

    //id do topico
    int topicid;

    //nome do topico
    char nome[50];

    //ponteiro para a primeira mensagem
    msg *primsg;
    pthread_mutex_t lock_msg;

    //ponteiro para o topico anterior
    //tpc * ant;

    //ponteiro para o topico seguinte
    tpc *prox;

    //array de ponteiros de users/clientes subscritos
    cltusr **subscritos;

    //numero de mensagens
    int nmensagens;
};

struct tpcpointers
{
    tpc *antaux, *aux, *proxaux;
};

//adiciona um novo topico
tpc addtpc(global *info, char *nome, int id);

//encontra topico pelo tid (topic id), retorna -1 no topicid se nao tiver encontrado
tpc findbytid(global *info, struct tpcpointers *pointers, int tid);

//vai buscar o ultimo topico, se nao existir returna null
tpc *getlasttpc(global *info);

//remove um topico pelo seu id, se ouverem mensagems ele nao ira apagar e ira returnar 0
int removebytid(global *info, int tid);

//verify if tid is available, returns 0 if it is, -1 if its not
int verifytid(global *info, int tid);

//MENSAGENS

//estrutura das mensagens
//no total as mensagens so podem ter 1000 chars
struct _msg
{

    //msgid
    int msgid;

    //topico da mensagem
    tpc *topicopointer; //ponteiro
    int topicoid;       //id

    //nao opbrigatorio
    char titulo[50];

    //corpo da mensagem
    char corpo[1000];

    //data e hora em que a mensagem foi enviada
    time_t sent;

    //tempo em que a mensagem deve ser apagada
    time_t deleted;

    //quem enviou
    pid_t authorpid;       //id
    cltusr *authorpointer; //ponteiro

    //mensagem anterior
    //msg * ant;

    //proxima mensagem
    msg *prox;
};

struct msgpointers
{
    msg *antaux, *aux, *proxaux;
};

//vai buscar a ultima mensagem
msg *getlastmsg(global *info);

msg sendmsg(global *info, char *titulo,int duracao , char *msg, int tpcid);

//re
void removebymid(global *info, int mid);

//verifica se  o mid 'e possivel e retorna 0 se nao encontrar, -1 se encontrar um igual
int verifymid(global *info, int mid);


#endif
