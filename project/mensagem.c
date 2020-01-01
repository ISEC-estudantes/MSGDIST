#include "./headers/mensagens.h"

/*
 * Trabalho de SO 2019 ISEC
 *
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 *
*/

//ALL - GENERIC TPC AND MSG MANAGEMENT

void addtpc(global *info, char *nome)
{


    pthread_mutex_lock(&info->lock_tpc);
    if (info->ntopicos == info->maxtopics) {
        wclear(info->notification);
        wprintw(info->notification, "Max topicos alcancados.");
        wrefresh(info->notification);
        pthread_mutex_unlock(&info->lock_tpc);
        return;
    }
    tpc *aux, *newtpc = malloc(sizeof(tpc));
    if (newtpc == NULL) {
        pthread_cancel(info->threads);
    }
    strcpy(newtpc->nome, nome);

    if ((aux = getlasttpc(info)) == NULL) {
        info->listtopicos = newtpc;
        aux->topicid = info->tidcounter = 0;
    } else {
        aux->prox = newtpc;
        int id = 0;
        while (verifytid(info, id++) == -1 || id != INT_MAX);
        if (id == INT_MAX) {
            wclear(info->notification);
            wprintw(info->notification, "porque %d topicos?!\nERA SUPOSTO SER UM PEQUENO TRABALHO DE SO!!!!!", INT_MAX);
            wrefresh(info->notification);
            pthread_mutex_unlock(&info->lock_tpc);
            return;
        }
        newtpc->topicid = --id;
        newtpc->prox = NULL;
        aux->prox = newtpc;
    }
    pthread_mutex_unlock(&info->lock_tpc);
}

int verifytid(global *info, int tid)
{
    tpc *aux = info->listtopicos;
    if (!aux) {
        return 0;
    }
    while (aux) {
        if (aux->topicid == tid) {
            return -1;
        }
    }
    return 0;
}

tpc *getlasttpc(global *info)
{
    tpc *aux, *proxaux;
    if (info->listtopicos) {
        aux = info->listtopicos;
        proxaux = aux->prox;
        while (proxaux) {
            aux = proxaux;
            proxaux = aux->prox;
        }
        return aux;
    }
    return NULL;
}

msg *getlastmsg(global *info)
{
    msg *aux, *proxaux;
    if (info->listtopicos) {
        aux = info->listtopicos;
        proxaux = aux->prox;
        while (proxaux) {
            aux = proxaux;
            proxaux = aux->prox;
        }
        return aux;
    }
    return NULL;
}

int removebytid(global *info, int tid)
{
    pthread_mutex_lock(&info->lock_tpc);

    struct tpcpointers pointers ;
    findbytid(info, &pointers, tid);
    //verificar se tem mensagens
    if (pointers.aux->nmensagens == 0) {

    }
    pthread_mutex_unlock(&info->lock_tpc);


}

tpc findbytid(global *info, struct tpcpointers *pointers, int tid)
{

    pointers->aux = pointers->proxaux = pointers->antaux = NULL;
    if (info->listtopicos) {
        if (info->debug == 1)
            printf("[removecliente]listclientes existe\n");

        pointers->aux = info->listtopicos;
        pointers->proxaux = pointers->aux->prox;
        while (pointers->proxaux && tid != pointers->aux->topicid) {
            pointers->antaux = pointers->aux;
            pointers->aux = pointers->proxaux;
            pointers->proxaux = pointers->proxaux->prox;
        }
        if (tid == pointers->aux->topicid) {
            return *pointers->aux;
        }
    }
    tpc anulado;
    anulado.topicid = 0;
    return anulado;
}
