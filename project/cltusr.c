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

cltusr searchbypid(global *info, struct _cltusrpointers *pointers, pid_t pid, int leave_unlocked)
{

    pointers->aux = pointers->proxaux = pointers->antaux = NULL;
    pthread_mutex_lock(&(info->lock_cltusr));
    if (info->listclientes) {
        if (info->debug == 1)
            printf("[removecliente]listclientes existe\n");
        pthread_mutex_lock(&info->lock_info);
        pointers->aux = info->listclientes;
        pthread_mutex_unlock(&info->lock_info);
        pointers->proxaux = pointers->aux->prox;
        while (pointers->proxaux && pid != pointers->aux->pid) {
            pointers->antaux = pointers->aux;
            pointers->aux = pointers->proxaux;
            pointers->proxaux = pointers->proxaux->prox;
        }
        if (pid == pointers->aux->pid) {
            if (leave_unlocked == 0)
                pthread_mutex_unlock(&(info->lock_cltusr));
            return *pointers->aux;
        }
    }
    pthread_mutex_unlock(&(info->lock_cltusr));
    cltusr anulado;
    anulado.pid = 0;
    return anulado;
};

int removebypid(global *info, int pid)
{
    struct _cltusrpointers pointers;
    if (searchbypid(info, &pointers, pid, 1).pid != 0) {
        //decrementa o numero de clientes
        --info->nclientes;
        if (pointers.proxaux) {
            if (pointers.antaux) {
                //existe um a frente e um atras
                pointers.antaux->prox = pointers.proxaux;
            } else {
                //existe um a frente mas nao existe um atras
                info->lastclient = pointers.antaux;
            }
        } else {
            if (pointers.antaux) {
                //nao a frente mas existe atras
                pointers.antaux->prox = NULL;
                info->lastclient = pointers.antaux;
            } else {
                info->lastclient = NULL;
                info->listclientes = NULL;
            }
        }
        free(pointers.aux);
        pthread_mutex_unlock(&info->lock_cltusr);
        return 0;
    } else {
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
        }
    }
    pthread_mutex_unlock(&info->lock_cltusr);
}

