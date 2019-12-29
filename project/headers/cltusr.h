#ifndef CLTUSR_H
#define CLTUSR_H

/*
 * Trabalho de SO 2019 ISEC
 * 
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 * 
*/
//ALL - GENERIC CLTUSR MANAGEMENT



typedef struct _client cltusr;

#include "utils.h"

struct _client
{

  //nome do user
  char nome[50];

  //id do cliente
  int pid;

  //cliente anterior
  //cltusr* ant;

  //proximo cliente
  cltusr * prox;
};

struct _cltusrpointers{
    cltusr *aux, *proxaux, *antaux;
};

//search a cliente by pid - if found returns 0 if not returns -1
//leave_unlocked - if 1 and it finds the cltusr it will leave the lock_cltusr unlock, else it will block it
cltusr searchbypid(global *info, struct _cltusrpointers *cltusrpointers, pid_t pid, int leave_unlocked);

//will remove the cltusr with the pid pid and use the function searchbypid as the search
int removebypid(global *info, int pid);

//will delete every cltusr that can be found in info->listclientes
void deleteallcltusr(global *info);


#endif

