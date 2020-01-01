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

struct _client {

    //nome do user
    char nome[50];

    //id do cliente
    int pid;
    
    //proximo cliente
    cltusr *prox;
};

struct cltusrpointers {
    cltusr *aux, *proxaux, *antaux;
};

#include "utils.h"



//search a cliente by pid - if found returns 0 if not returns -1
cltusr searchbypid(global *info, struct cltusrpointers *cltusrpointers, pid_t pid);

//will remove the cltusr with the pid pid and use the function searchbypid as the search
int removebypid(global *info, int pid);

//will delete every cltusr that can be found in info->listclientes
void deleteallcltusr(global *info);

//vai buscar o ultimo cltusr e retorna o seu ponteiro, se nao existir nenhum returna um null
cltusr * getlastcltusr(global * info);

#endif

