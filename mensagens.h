#ifndef MENSAGENS_H
#define MENSAGENS_H

#include "utils.h"

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
    msg * primsg;

    //ponteiro para o topico anterior
    tpc * ant;

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

    //ponteiro para o topico da mensagem
     tpc * topico;

    //nao opbrigatorio?
    char  titulo[50];

    //corpo da mensagem
    char * corpo[1000];

    //tempo maximo que a mensagem fica guardada(tempo em segundos)
    int duracao;

    //quem enviou
    cltusr * author;
    
    //mensagem anterior
    msg * ant;

    //proxima mensagem
    msg * prox;

};

#endif
