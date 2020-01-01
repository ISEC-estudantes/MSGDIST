/*
 * Trabalho de SO 2019 ISEC
 *
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 *
*/
//ALL - GENERIC CLTUSR MANAGEMENT

#include "./headers/cltusr.h"

cltusr searchbypid(global *info, struct cltusrpointers *pointers, pid_t pid)
{

    pointers->aux = pointers->proxaux = pointers->antaux = NULL;
    if (info->listclientes) {
        if (info->debug == 1)
            printf("[removecliente]listclientes existe\n");

        pointers->aux = info->listclientes;
        pointers->proxaux = pointers->aux->prox;
        while (pointers->proxaux && pid != pointers->aux->pid) {
            pointers->antaux = pointers->aux;
            pointers->aux = pointers->proxaux;
            pointers->proxaux = pointers->proxaux->prox;
        }
        if (pid == pointers->aux->pid) {
            return *pointers->aux;
        }
    }
    cltusr anulado;
    anulado.pid = 0;
    return anulado;
};

int removebypid(global *info, int pid)
{
    if (info->debug == 1)
        printf("[REMOVEBYPID]unlocked\n");
    pthread_mutex_lock(&info->lock_info);
    if (info->debug == 1)
        printf("[REMOVEBYPID]locked\n");
    struct cltusrpointers pointers;
    if (searchbypid(info, &pointers, pid).pid != 0) {
        //decrementa o numero de clientes
        --info->nclientes;
        if (pointers.proxaux) {
            if (pointers.antaux) {
                //existe um a frente e um atras
                pointers.antaux->prox = pointers.proxaux;
            } else {
                //existe um a frente mas nao existe um atras
            }
        } else {
            if (pointers.antaux) {
                //nao a frente mas existe atras
                pointers.antaux->prox = NULL;
            } else {
                info->listclientes = NULL;
            }
        }
        free(pointers.aux);
        pthread_mutex_unlock(&info->lock_cltusr);
        return 0;
    } else {
        pthread_mutex_unlock(&info->lock_cltusr);
        return -1;
    }

};

void deleteallcltusr(global *info)
{
    pthread_mutex_lock(&info->lock_cltusr);
    if (info->listclientes) {
        cltusr *aux = info->listclientes, *prox = NULL;
        while (aux) {
            prox = aux->prox;
            free(aux);
            aux = prox;
            --(info->nclientes);
        }
        info->listclientes = NULL;
    }
    pthread_mutex_unlock(&info->lock_cltusr);
}

cltusr *getlastcltusr(global *info)
{
    cltusr *aux, *proxaux;
    if (info->listclientes) {
        aux = info->listclientes;
        proxaux = aux->prox;
        while (proxaux) {
            aux = proxaux;
            proxaux = aux->prox;
        }
        return aux;
    }
    return NULL;
}
