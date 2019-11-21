
#ifndef CLIENTE_H
#define CLIENTE_H
#pragma once


typedef struct _client cltusr;


struct _client
{

  //nome do user
  char * nome;

  //id do cliente
  int pid;

  //cliente anterior
  cltusr* ant;

  //proximo cliente
  cltusr * prox;

};
#endif

