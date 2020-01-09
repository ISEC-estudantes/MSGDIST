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

tpc addtpc(global *info, char *nome, int id)
{
    tpc temptpc;
    pthread_mutex_lock(&info->lock_tpc);
    if (info->ntopicos == info->maxtopics)
    {
        wclear(info->notification);
        wprintw(info->notification, "Max topicos alcancados.");
        wrefresh(info->notification);
        pthread_mutex_unlock(&info->lock_tpc);
        temptpc.topicid = -1;
        return temptpc;
    }
    tpc *aux, *newtpc = malloc(sizeof(tpc));
    if (newtpc == NULL)
    {
        pthread_cancel(info->threads);
    }
    strcpy(newtpc->nome, nome);

    if ((aux = getlasttpc(info)) == NULL)
    {
        info->listtopicos = newtpc;
        aux->topicid = 0;
        return *newtpc;
    }
    else
    {
        aux->prox = newtpc;
        int id = 0;
        if (id == INT_MAX)
        {
            wclear(info->notification);
            wprintw(info->notification, "porque %d topicos?!\nERA SUPOSTO SER UM PEQUENO TRABALHO DE SO!!!!!", INT_MAX);
            wrefresh(info->notification);
            pthread_mutex_unlock(&info->lock_tpc);
            newtpc->topicid = -1;
            return *newtpc;
        }
        newtpc->topicid = --id;
        newtpc->prox = NULL;
        aux->prox = newtpc;
        return *newtpc;
    }
    pthread_mutex_unlock(&info->lock_tpc);
}

int verifytid(global *info, int tid)
{
    tpc *aux = info->listtopicos;

    while (aux)
    {
        if (aux->topicid == tid)
        {
            return -1;
        }
        aux = aux->prox;
    }
    return 0;
}

int verifymid(global *info, int mid)
{
    tpc *taux = info->listtopicos;
    msg *aux = NULL;

    while (taux)
    {
        while (aux)
        {
            if (aux->msgid == mid)
            {
                return -1;
            }
            aux = aux->prox;
        }
        taux = taux->prox;
    }
    return 0;
}

tpc *getlasttpc(global *info)
{
    tpc *aux, *proxaux;
    if (info->listtopicos)
    {
        aux = info->listtopicos;
        proxaux = aux->prox;
        while (proxaux)
        {
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
    if (info->listtopicos)
    {
        aux = info->listtopicos;
        proxaux = aux->prox;
        while (proxaux)
        {
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

    struct tpcpointers pointers;
    findbytid(info, &pointers, tid);

    pthread_mutex_unlock(&info->lock_tpc);
}

tpc findbytid(global *info, struct tpcpointers *pointers, int tid)
{

    pointers->aux = pointers->proxaux = pointers->antaux = NULL;
    if (info->listtopicos)
    {
        if (info->debug == 1)
            printf("[removecliente]listclientes existe\n");

        pointers->aux = info->listtopicos;
        pointers->proxaux = pointers->aux->prox;
        while (pointers->proxaux && tid != pointers->aux->topicid)
        {
            pointers->antaux = pointers->aux;
            pointers->aux = pointers->proxaux;
            pointers->proxaux = pointers->proxaux->prox;
        }
        if (tid == pointers->aux->topicid)
        {
            return *pointers->aux;
        }
    }
    tpc anulado;
    anulado.topicid = -1;
    return anulado;
}

//remover uma mensagem pelo o seu id
void removebymid(global *info, int mid)
{

    pthread_mutex_lock(&info->lock_tpc);
    tpc *taux = info->listtopicos;
    msg *maux = NULL, *mauxant;
    while (taux)
    {
        maux = taux->primsg;
        mauxant = NULL;
        while (maux)
        {
            if (maux->msgid == mid)
            {
                if (mauxant == NULL)
                {
                    taux->primsg = maux->prox;
                }
                else
                {
                    mauxant->prox = maux->prox;
                }
                free(maux);
                pthread_mutex_unlock(&info->lock_tpc);
                return;
            }
        }
        taux = taux->prox;
    }
    pthread_mutex_unlock(&info->lock_tpc);
}
