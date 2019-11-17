
#ifndef CLIENTE_H
#define CLIENTE_H



typedef struct _client cliente;


int NCLIENTES;

struct _client
{

  //nome do user
  char * nome;

  //id do cliente
  int pid;
  
  //ponteiro para o user que abrio o cliente
  user * conta;

  //cliente anterior
  cliente * ant;

  //proximo cliente
  cliente * prox;

};
#endif

