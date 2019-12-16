#include "threads.h"

typedef struct _sendthings sendthings;
struct _sendthings
{
    global *info;
    void *temporary;
};

void readingfifo(void *input)
{
    sendthings temporary;
    pthread_t handler_creat;
    global *info = (global *)input;

    int gestorfifo = open("gestorfifo", O_RDONLY), recebe = 0;
    cltusr addclientestruct;
    msg addmsgstruct;
    while (info->terminate != 1)
    {
        read(gestorfifo, (void *)&recebe, sizeof(int));
        if (recebe == ADD_CLIENT)
        {
            read(gestorfifo, (void *)&addclientestruct, sizeof(cltusr));
            pthread_create(&handler_creat, NULL, (void*)addcliente, (void *)&temporary);
        }
    }
}

void addcliente(void *received)
{
    sendthings *utils = ((sendthings *)received);
    global *info = utils->info;
    cltusr addclientestruct = *((cltusr *)utils->temporary);

    cltusr *aux = info->listclientes, *antaux = NULL;
    int fd;
    char *cliente;
    sprintf(cliente, "%d", addclientestruct.pid);

    if (info->maxusers == info->nclientes)
    {
        fd = open(cliente, O_WRONLY);
        write(fd, (void*)(int)-2, sizeof(int));
        close(fd);
        return;
    }

    if (aux == NULL)
    {
        info->listclientes = (cltusr *)malloc(sizeof(cltusr));
        if (!info->listclientes)
        {
            freethings(info);
            return;
        }
        aux = info->listclientes;
        strcpy(aux->nome, addclientestruct.nome);
        aux->pid = addclientestruct.pid;
        aux->ant = NULL;
        aux->prox = NULL;
        ++(info->nclientes);
    }
    else
    {
        //verificacao do nome
        if (nomecheck(info, cliente, &addclientestruct) != 0)
            return;

        while (aux != NULL)
        {
            antaux = aux;
            aux = aux->prox;
        }
        antaux->prox = malloc(sizeof(cltusr));
        aux = antaux->prox;
        if (!aux)
        {
            semmem();
            freethings(info);
        }
        strcpy(aux->nome, addclientestruct.nome);
        aux->pid = addclientestruct.pid;
        aux->ant = antaux;
        aux->prox = NULL;
        ++(info->nclientes);
    }
}

int nomecheck(global *info, char *cliente, cltusr *aux)
{
    int i, numint, len, fd;
    cltusr *actual = info->listclientes;
    char numchar[3];

    while (actual)
    {
        if (strcmp(aux->nome, actual->nome) == 0)
            len = strlen(aux->nome);
        if (
            aux->nome[len - 3] >= '0' &&
            aux->nome[len - 3] <= '9' &&
            aux->nome[len - 2] >= '0' &&
            aux->nome[len - 2] <= '9' &&
            aux->nome[len - 1] >= '0' &&
            aux->nome[len - 1] <= '9')
        {
            numchar[0] = aux->nome[len - 3];
            numchar[1] = aux->nome[len - 2];
            numchar[2] = aux->nome[len - 1];
            numint = atoi(numchar);
            if (numint < 999)
                ++numint;
            else
            {
                fd = open(cliente, O_WRONLY);
                write(fd, (void*)((int)-1), sizeof(int));
                close(fd);
            }
        }
        else
        {
            strcat(aux->nome, "001");
        }

        actual = actual->prox;
    }
    return 0;
}

void intapagaclientes(global *info)
{
    if (info->listclientes != NULL)
    {
        cltusr *aux = info->listclientes->prox, *auxant = info->listclientes;

        while (aux != NULL)
        {
            auxant = aux;
            aux = auxant->prox;
        }
        aux = auxant;
        while (auxant != NULL)
        {
            aux = auxant;
            auxant = auxant->ant;
            free(aux);
        }
    }
}

void freethings(global * info)
{
    info->terminate = 1;
    pthread_join(info->read_fifo, NULL);
    if (info != NULL)
    {
        killverifica(info->cpid);
        if (info->listclientes != NULL)
        {
            cltusr *auxc = info->listclientes, *proxauxc;
            proxauxc = auxc->prox;
            while (auxc)
            {
                free(auxc);
                auxc = proxauxc;
                if (auxc)
                    proxauxc = auxc->prox;
            }
        }
        if (info->listtopicos)
        {
            tpc *auxt = info->listtopicos, *proxauxt;
            msg *auxm, *proxauxm;
            proxauxt = auxt->prox;
            while (auxt)
            {
                if (auxt->primsg)
                {
                    auxm = auxt->primsg;
                    proxauxm = auxm->prox;
                    while (auxm)
                    {
                        free(auxm);
                        auxm = proxauxm;
                        if (auxm)
                            proxauxm = auxm->prox;
                    }
                }

                free(auxt);
                auxt = proxauxt;
                if (auxt)
                    proxauxt = auxt->prox;
            }
        }
        free(info);
    }
}
