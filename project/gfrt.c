/*
 * Trabalho de SO 2019 ISEC
 *
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 *
*/
//GESTOR - FIFO READER THREAD(S)

#include "./headers/gfrt.h"
#include "./headers/verificar.h"

typedef struct _sendthings sendthings;

void readingfifo(void *input)
{
    //pthread_t handler_creat;
    global *info = (global *)input;
    int gestorfifo = open(GESTORFIFO, O_RDWR), bytes, lixofifo;
    pipemsg recebe;
    gestorfifo = open(GESTORFIFO, O_RDONLY);
    lixofifo = open(GESTORFIFO, O_WRONLY);
    if (gestorfifo < 0)
    {
        printf("erro[%d] a abrir o fifo.\n", gestorfifo);
        pthread_cancel(info->threads);
        pthread_join(info->threads, NULL);
    }

    while (info->terminate != 1)
    {
        if (info->debug == 1)
            printf("[THREAD_READINGFIFO] new round\n");

        bytes = read(gestorfifo, (void *)&recebe, sizeof(pipemsg));
        if (info->debug == 1)
            printf("[THREAD_READINGFIFO] bytes recebidos =%d, codigo = %d, pid = %d \n", bytes, recebe.codigo, recebe.pid);

        switch (recebe.codigo)
        {
        case ADD_CLIENT:
            if (info->debug == 1)
                printf("cliente -> nome = %s\n", recebe.clientname);
            addcliente(info, &recebe);
            break;
        case CLOSING_CLIENT:
            if (info->debug == 1)
                printf("cliente a fechar\n");
            removebypid(info, recebe.pid);
            break;
        case ADD_TOPIC:
            addtopic(info, recebe);
            break;
        case ADD_MESSAGE:
            addmensagem(info, recebe);
            break;
        case SHUTDOWN:
            //terminating....
            break;
        }
    }
    close(gestorfifo);
}

//adiciona uma mensagem
void addmensagem(global *info, pipemsg rcvenv)
{
    char pidcliente_char[10];
    int id = 0, ff;
    sprintf(pidcliente_char, "%d", rcvenv.pid);
    pthread_mutex_lock(&info->lock_tpc);
    while (verifymid(info, id++) == -1 || id != INT_MAX)
        ;
    if (id == INT_MAX)
    {
        fprintf(stderr, "n de topicos é igual ao numero maximo de ints.\n");
        return;
    }
    ff = open(pidcliente_char, O_WRONLY);
    if (ff == -1)
    {
        fprintf(stderr, "problemas a abrir o fifo\n");
        kill(rcvenv.pid, SIGUSR2);
    }

    if (info->maxtopics == info->ntopicos)
    {
        fprintf(stderr, " numero de cliente demasiado alto para adicionar o topico de nome %s\n", rcvenv.titulo);
        rcvenv.codigo = INVALID_CLIENT_MAX;

        return;
    }
    rcvenv.topicid = --id;
    write(ff, (void *)&rcvenv, sizeof(pipemsg));
    close(ff);
    addtpc(info, rcvenv.titulo, rcvenv.topicid);
}

//adiciona um topico
void addtopic(global *info, pipemsg rcvenv)
{
    char pidcliente_char[10];
    int id = 0, ff;
    sprintf(pidcliente_char, "%d", rcvenv.pid);
    pthread_mutex_lock(&info->lock_tpc);
    while (verifytid(info, id++) == -1 || id != INT_MAX)
        ;
    if (id == INT_MAX)
    {
        fprintf(stderr, "n de topicos é igual ao numero maximo de ints.\n");
        return;
    }
    ff = open(pidcliente_char, O_WRONLY);
    if (ff == -1)
    {
        fprintf(stderr, "problemas a abrir o fifo\n");
        kill(rcvenv.pid, SIGUSR2);
    }

    if (info->maxtopics == info->ntopicos)
    {
        fprintf(stderr, " numero de cliente demasiado alto para adicionar o topico de nome %s\n", rcvenv.titulo);
        rcvenv.codigo = INVALID_CLIENT_MAX;

        return;
    }
    rcvenv.topicid = --id;
    write(ff, (void *)&rcvenv, sizeof(pipemsg));
    close(ff);
    addtpc(info, rcvenv.titulo, rcvenv.topicid);
}

void addcliente(global *info, pipemsg *recebe)
{
    int nclts, fd, erro = 0;
    char pidcliente_char[10];
    sprintf(pidcliente_char, "%d", recebe->pid);
    pipemsg enviar;
    pthread_mutex_lock(&info->lock_cltusr);
    nclts = info->nclientes;
    pthread_mutex_unlock(&info->lock_cltusr);
    if (info->maxusers == nclts)
        erro = 1;
    if (erro == 1)
    {
        fd = open(pidcliente_char, O_WRONLY);
        if (fd == -1)
        {
            //matar o cliente
            kill(recebe->pid, SIGINT);
        }
        else
        {
            enviar.codigo = INVALID_CLIENT_MAX;
            write(fd, (void *)&enviar.codigo, sizeof(pipemsg));
            close(fd);
        }
        return;
    }

    //ajustar o nome para enters e espacos
    char *nm = recebe->clientname;
    int len = strlen(recebe->clientname);
    len--;
    while ((
               nm[len] == ' ' ||
               nm[len] == '\t' ||
               nm[len] == '\n') &&
           len > 0)
    {
        nm[len--] = '\0';
    }

    //verificacao do nome
    if (nclts > 0)
        if (nomecheck(info, recebe->clientname, recebe->pid) == NULL)
            return;

    //enviar resposta

    fd = open(pidcliente_char, O_WRONLY);
    if (fd == -1)
    {
        kill(recebe->pid, SIGINT);
        return;
    }

    recebe->codigo = 0;
    write(fd, (void *)recebe, sizeof(pipemsg));
    if (info->debug == 1)
        printf("[THREAD_READINGFIFO] bytes recebidos = checked, codigo = %d, pid = %d , nome = %s\n", recebe->codigo, recebe->pid, recebe->clientname);
    close(fd);

    //gestao de memoria

    pthread_mutex_lock(&info->lock_cltusr);
    cltusr *aux = (cltusr *)malloc(sizeof(cltusr));
    if (!aux)
    {
        semmem();
        pthread_cancel(info->threads);
    }
    strcpy(aux->nome, recebe->clientname);
    aux->pid = recebe->pid;
    aux->prox = NULL;
    if (nclts == 0)
    {

        //caso ainda nao exista numgem

        info->listclientes = aux;
        ++(info->nclientes);
    }
    else
    {
        //caso existem mais
        cltusr *last = getlastcltusr(info);
        if (last)
        {
            last->prox = aux;
            ++(info->nclientes);
        }
    }

    pthread_mutex_unlock(&info->lock_cltusr);
    //fim de gestao
}

char *nomecheck(global *info, char *cliente, int pid)
{
    if (info->debug == 1)
        printf("[THREAD ADD CLIENTE] verify name\n");

    int i, numint, len, fd;

    char numchar[4];

    //buscar todos os nomes para um array
    pthread_mutex_lock(&info->lock_cltusr);
    int nclts = info->nclientes;
    if (nclts < 1)
    {
        pthread_mutex_unlock(&info->lock_cltusr);
        return cliente;
    }
    cltusr *actual = info->listclientes;
    char nomes[nclts][50];
    for (i = 0; i < nclts && actual; ++i)
    {
        strcpy(nomes[i], actual->nome);
        actual = actual->prox;
    }
    pthread_mutex_unlock(&info->lock_cltusr);
    nclts = i;
    for (i = 0; i < nclts; ++i)
    {
        if (strcmp(nomes[i], cliente) == 0)
        {
            len = strlen(nomes[i]);
            //esta parte serve para identificar se os ultimos char sao numeros
            if (
                nomes[i][len - 3] >= '0' &&
                nomes[i][len - 3] <= '9' &&
                nomes[i][len - 2] >= '0' &&
                nomes[i][len - 2] <= '9' &&
                nomes[i][len - 1] >= '0' &&
                nomes[i][len - 1] <= '9')
            {
                numchar[0] = nomes[i][len - 3];
                numchar[1] = nomes[i][len - 2];
                numchar[2] = nomes[i][len - 1];
                numchar[3] = '\0';
                numint = atoi(numchar);
                if (info->debug == 1)
                    fprintf(stderr, "nomes[i] = %s; numint = %d\n", nomes[i], numint);
                if (numint < 999)
                {
                    sprintf(numchar, "%03d", ++numint);
                    numchar[3] = '\0';
                    if (info->debug == 1)
                        fprintf(stderr, "numchar = %s\n", numchar);
                    cliente[len - 3] = numchar[0];
                    cliente[len - 2] = numchar[1];
                    cliente[len - 1] = numchar[2];
                }

                else
                {
                    char pidcliente_char[10];
                    sprintf(pidcliente_char, "%d", pid);
                    fd = open(pidcliente_char, O_WRONLY);
                    if (fd == -1)
                    {
                        kill(pid, SIGINT);
                    }
                    else
                    {
                        pipemsg enviar;
                        enviar.codigo = INVALID_CLIENT_NAME;
                        write(fd, (void *)&enviar, sizeof(pipemsg));
                        close(fd);
                    }
                    return NULL;
                }
            }
            else
            {
                strcat(cliente, "001");
            }
        }
    }
    return cliente;
}

void freethings(global *info)
{
    char pidchar[10];
    int ff_cliente;
    pipemsg envrcb;
    envrcb.codigo = SHUTDOWN;
    printf("verificar o info --FREETHINGS-- \n");
    printf("info->cpid %d", info->cpid);
    printf("end da verifcacao\n");
    info->terminate = 1;
    if (info != NULL)
    {
        killverifica(info->cpid);
        if (info->listclientes != NULL)
        {
            cltusr *auxc = info->listclientes, *proxauxc;
            proxauxc = auxc->prox;
            while (auxc)
            {
                sprintf(pidchar, "%d", auxc->pid);
                ff_cliente = open(pidchar, O_RDONLY | O_NONBLOCK);
                if (ff_cliente > -1)
                {
                    write(ff_cliente, (void *)&envrcb, sizeof(envrcb));
                }
                else
                {
                    kill(auxc->pid, SIGINT);
                }
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
        info = NULL;
    }
    unlink(GESTORFIFO);
    exit(0);
}

void timerdeleter(void *input)
{
    global *info = (global *)input;
    time_t rightnow = 0;
    tpc *auxt = NULL;
    msg *auxm = NULL, *antauxm;

    while (1)
    {
        sleep(1);
        rightnow = time(NULL);
        pthread_mutex_lock(&info->lock_tpc);
        auxt = info->listtopicos;
        while (auxt)
        {
            auxm = auxt->primsg;
            antauxm = NULL;
            while (auxm)
            {
                if (auxm->deleted == rightnow)
                {
                    if (antauxm)
                        antauxm->prox = auxm->prox;
                    else
                        auxt->primsg = auxm->prox;
                    auxm = auxm->prox;
                    --(auxt->nmensagens);
                    free(auxm);
                }
                auxm = auxm->prox;
            }
        }
        pthread_mutex_unlock(&info->lock_tpc);
    }
}