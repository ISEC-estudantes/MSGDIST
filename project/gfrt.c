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
    global *info = (global *) input;
    int gestorfifo = open(GESTORFIFO, O_RDWR);
    pipemsg recebe;
    gestorfifo = open(GESTORFIFO, O_RDONLY);
    int lixofifo = open(GESTORFIFO, O_WRONLY);
    if (gestorfifo < 0) {
        printf("erro[%d] a abrir o fifo.\n", gestorfifo);
        freethings(info);
    }
    unsigned int pid;

    while (info->terminate != 1) {
        //close(gestorfifo);
        if (info->debug == 1)
            printf("[THREAD_READINGFIFO] new round\n");

        read(gestorfifo, (void *) &recebe, sizeof(pipemsg));


        if (recebe.codigo == ADD_CLIENT) {
            if (info->debug == 1)
                printf("cliente -> nome = %s\n", recebe.clientname);
            addcliente(info, &recebe);
        } else if (recebe.codigo == CLOSING_CLIENT) {
            read(gestorfifo, (void *) &pid, sizeof(int));
            if (info->debug == 1)
                printf("cliente a fechar\n");
            removebypid(info, pid);
        } else if (recebe.codigo == SHUTDOWN) {
            //terminating....
            break;
        }
    }
    close(gestorfifo);
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
    if (erro == 1) {
        fd = open(pidcliente_char, O_WRONLY);
        if (fd == -1) {
            //matar o cliente
            kill(recebe->pid, SIGINT);
        } else {
            enviar.codigo = INVALID_CLIENT_MAX;
            write(fd, (void *) &enviar.codigo, sizeof(pipemsg));
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
            len > 0) {
        nm[len--] = '\0';
    }

    //verificacao do nome
    if (nclts > 0)
        if (nomecheck(info, recebe->clientname, recebe->pid) == NULL)
            return;

    pthread_mutex_lock(&info->lock_cltusr);
    cltusr *aux = info->listclientes;
    if (nclts == 0) {
        fd = open(pidcliente_char, O_WRONLY);
        if (fd == -1) {
            kill(recebe->pid, SIGINT);
            
        }
        write(fd, (void *) recebe, sizeof(pipemsg));
        close(fd);
        info->listclientes = (cltusr *) malloc(sizeof(cltusr));
        if (!info->listclientes) {
            semmem();
            freethings(info);
            return;
        }
        aux = info->listclientes;
        strcpy(aux->nome, recebe->clientname);
        aux->pid = recebe->pid;
        aux->prox = NULL;
        ++ (info->nclientes);
        
    } else {
        fd = open(pidcliente_char, O_WRONLY);
        if (fd == -1) {
            printf("Problemas a contactar o cliente... a terminar lo.\n");
            kill(recebe->pid, SIGINT);
            return;
        }
        write(fd, (void *) recebe, sizeof(pipemsg));
        close(fd);
        info->lastclient->prox = malloc(sizeof(cltusr));
        if (info->lastclient->prox) {
            semmem();
            freethings(info);
        }
        aux = info->lastclient->prox;
        strcpy(aux->nome, recebe->clientname);
        aux->pid = recebe->pid;
        aux->prox = NULL;
        info->lastclient = aux;
        ++ (info->nclientes);
    }
    pthread_mutex_unlock(&info->lock_cltusr);
}

char *nomecheck(global *info, char *cliente, int pid)
{
    int i, numint, len, fd, erro;

    char numchar[3];

    //buscar todos os nomes para um array
    pthread_mutex_lock(&info->lock_cltusr);
    int nclts = info->nclientes;
    if (nclts < 1) {
        pthread_mutex_unlock(&info->lock_cltusr);
        return cliente;
    }
    cltusr *actual = info->listclientes;
    char nomes[nclts][50];
    for (i = 0; i < nclts && actual; ++i) {
        strcpy(nomes[i], actual->nome);
        actual = actual->prox;
    }
    pthread_mutex_unlock(&info->lock_cltusr);
    nclts = i;
    for (i = 0; i < nclts; ++i) {
        if (strcmp(nomes[i], cliente) == 0) {
            len = strlen(nomes[i]);
            //esta parte serve para identificar se os ultimos char sao numeros
            if (
                nomes[i][len - 3] >= '0' &&
                nomes[i][len - 3] <= '9' &&
                nomes[i][len - 2] >= '0' &&
                nomes[i][len - 2] <= '9' &&
                nomes[i][len - 1] >= '0' &&
                nomes[i][len - 1] <= '9') {
                numchar[0] = nomes[i][len - 3];
                numchar[1] = nomes[i][len - 2];
                numchar[2] = nomes[i][len - 1];
                numint = atoi(numchar);
                if (info->debug == 1)
                    fprintf(stderr, "nomes[i] = %s; numint = %d\n", nomes[i], numint);
                if (numint < 999) {
                    sprintf(numchar, "%03d", ++numint);
                    if (info->debug == 1)
                        fprintf(stderr, "numchar = %s\n", numchar);
                    strcat(cliente, numchar);
                }

                else {
                    char pidcliente_char[10];
                    sprintf(pidcliente_char, "%d", pid);
                    fd = open(pidcliente_char, O_WRONLY);
                    if (fd == -1) {
                        kill(pid, SIGINT);
                    } else {
                        pipemsg enviar;
                        enviar.codigo = INVALID_CLIENT_NAME;
                        write(fd, (void *) &enviar, sizeof(pipemsg));
                        close(fd);
                    }
                    return NULL;
                }
            } else {
                strcat(cliente, "001");
            }
        }
    }
    return cliente;
}

void freethings(global *info)
{
    printf("verificar o info --FREETHINGS-- \n");
    printf("info->cpid %d", info->cpid);
    printf("end da verifcacao\n");
    info->terminate = 1;
    pthread_cancel(info->read_fifo);
    pthread_join(info->read_fifo, NULL);
    if (info != NULL) {
        killverifica(info->cpid);
        if (info->listclientes != NULL) {
            cltusr *auxc = info->listclientes, *proxauxc;
            proxauxc = auxc->prox;
            while (auxc) {
                kill(auxc->pid, SIGINT);
                free(auxc);
                auxc = proxauxc;
                if (auxc)
                    proxauxc = auxc->prox;
            }
        }
        if (info->listtopicos) {
            tpc *auxt = info->listtopicos, *proxauxt;
            msg *auxm, *proxauxm;
            proxauxt = auxt->prox;
            while (auxt) {
                if (auxt->primsg) {
                    auxm = auxt->primsg;
                    proxauxm = auxm->prox;
                    while (auxm) {
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
