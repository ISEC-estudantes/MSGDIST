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
//estrutura das mensagens
//no total as mensagens so podem ter 1000 chars
struct _tpc{

    //id do topico
    int topicid;
    
    //nome do topico
    char nome[50];

    //ponteiro para a primeira mensagem
    msg * primsg, *lastmsg;
    pthread_mutex_t lock_msg;

    //ponteiro para o topico anterior
    //tpc * ant;

    //ponteiro para o topico seguinte
    tpc * prox;

    //array de ponteiros de users/clientes subscritos
    cltusr ** subscritos;

    //numero de mensagens
    int nmensagens;

};

struct _msg{

    //msgid
    int msgid;

    //topico da mensagem
     tpc * topicopointer;//ponteiro
     int topicoid;//id
     
    //nao opbrigatorio
    char  titulo[50];

    //corpo da mensagem
    char corpo[1000];

    //data e hora em que a mensagem foi enviada
    time_t sent;
    
    //tempo em que a mensagem deve ser apagada
    time_t deleted;
    
    //quem enviou
    pid_t authorpid;//id
    cltusr * authorpointer;//ponteiro
    
    //mensagem anterior
    //msg * ant;

    //proxima mensagem
    msg * prox;
    
    

};



#endif
