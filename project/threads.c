#include "./headers/threads.h"
#include "./headers/verificar.h"

typedef struct _sendthings sendthings;


void readingfifo ( void *input ) {
    pthread_t handler_creat;
    global *info = ( global * ) input;
    int gestorfifo = open ( GESTORFIFO, O_RDWR ), recebe, n, loop = 0;
    gestorfifo = open ( GESTORFIFO, O_RDONLY );
    int lixofifo = open ( GESTORFIFO, O_WRONLY );
    if ( gestorfifo < 0 ) {
        printf ( "erro[%d] a abrir o fifo.\n", gestorfifo );
        freethings ( info );
    }
    unsigned int pid;

    cltusr addclientestruct;// *auxc, *antauxc, *proxauxc;
    while ( info->terminate != 1 ) {
        //close(gestorfifo);
        if ( info->debug == 1 )
            printf ( "[THREAD_READINGFIFO] new round\n" );
        recebe = 0;

        n = read ( gestorfifo, ( void * ) &recebe, sizeof ( int ) );

        if ( info->debug == 1 ) {
            if ( n > 0 ) {
                printf ( "recebe = %d\n", n );
                loop = 0;
            } else if ( loop == 0 ) {
                printf ( "in loop...\n" );
                loop = 1;
            }
        }

        if ( recebe == ADD_CLIENT ) {
            if ( info->debug == 1 )
                printf ( "a ler cliente...\n" );
            read ( gestorfifo, ( void * ) &addclientestruct, sizeof ( cltusr ) );
            if ( info->debug == 1 )
                printf ( "cliente -> nome = %s\n", addclientestruct.nome );
            info->temppointer = ( void * ) &addclientestruct;
            pthread_create ( &handler_creat, NULL, ( void * ) addcliente, ( void * ) &info );
            //addcliente((void *)&info);
        } else if ( recebe == CLOSING_CLIENT ) {
            read ( gestorfifo, ( void * ) &pid, sizeof ( int ) );
            if ( info->debug == 1 )
                printf ( "cliente a fechar\n" );
            info->tempint = pid;
            pthread_create ( &handler_creat, NULL, ( void * ) removecliente, ( void* ) &info );
        } else if ( recebe == 0 ) {
            printf ( "recebemos 0\n" );
        }
    }
    close ( gestorfifo );
}

void removecliente ( void * input ) {
    //receber coisas
    global * info = (global *)input;
    int pid = info->tempint;

    cltusr *auxc, *proxauxc, *antauxc;
    if ( info->listclientes ) {
        if ( info->debug == 1 )
            printf ( "listclientes existe\n" );
        auxc = info->listclientes;
        proxauxc = auxc->prox;
        while ( proxauxc && pid != auxc->pid ) {
            auxc = proxauxc;
            proxauxc = proxauxc->prox;
        }
        if ( pid == auxc->pid ) {
            antauxc = auxc->ant;
            if ( info->listclientes == auxc ) {
                info->listclientes = proxauxc;
                if ( proxauxc )
                    proxauxc->ant = NULL;
            }
            if ( antauxc )
                antauxc->prox = proxauxc;
            if ( proxauxc )
                proxauxc->ant = antauxc;
            free ( auxc );
        }
    }
}

void addcliente ( void *input ) {
    
    global *info = (global *)input;
    cltusr *addclientestruct = ( ( cltusr * ) info->temppointer);

    cltusr *aux = info->listclientes, *antaux = NULL;
    int fd, erro;
    char cliente[10];
    sprintf ( cliente, "%d", addclientestruct->pid );

    if ( info->maxusers == info->nclientes ) {
        fd = open ( cliente, O_WRONLY );
        if ( fd == -1 ) {
            kill ( addclientestruct->pid, SIGINT );
        } else {
            erro = INVALID_CLIENT_MAX;
            write ( fd, ( void * ) &erro, sizeof ( int ) );
            close ( fd );
        }
        return;
    }

    //ajustar o nome para enters e espacos
    char *nm = addclientestruct->nome;
    int len = strlen ( addclientestruct->nome );
    len--;
    while ( (
                nm[len] == ' ' ||
                nm[len] == '\t' ||
                nm[len] == '\n' ) &&
            len > 0 ) {
        nm[len--] = '\0';
    }

    if ( aux == NULL ) {
        info->listclientes = ( cltusr * ) malloc ( sizeof ( cltusr ) );
        if ( !info->listclientes ) {
            freethings ( info );
            return;
        }
        aux = info->listclientes;
        strcpy ( aux->nome, addclientestruct->nome );
        aux->pid = addclientestruct->pid;
        aux->ant = NULL;
        aux->prox = NULL;
        ++ ( info->nclientes );
        fd = open ( cliente, O_WRONLY );
        erro = 0;
        write ( fd, ( void * ) &erro, sizeof ( int ) );
        write ( fd, ( void * ) &addclientestruct->nome, sizeof ( char ) * 256 );
    } else {
        //verificacao do nome
        if ( nomecheck ( info, cliente, addclientestruct ) != 0 )
            return;

        while ( aux != NULL ) {
            antaux = aux;
            aux = aux->prox;
        }
        antaux->prox = malloc ( sizeof ( cltusr ) );
        aux = antaux->prox;
        if ( !aux ) {
            semmem();
            freethings ( info );
        }
        strcpy ( aux->nome, addclientestruct->nome );
        aux->pid = addclientestruct->pid;
        aux->ant = antaux;
        aux->prox = NULL;
        ++ ( info->nclientes );
        fd = open ( cliente, O_WRONLY );
        if ( fd == -1 ) {
            printf ( "Problemas a contactar o cliente... a terminar lo.\n" );
            kill ( addclientestruct->pid, SIGINT );
            info->tempint = addclientestruct->pid ;
            removecliente ( (void*)info);
        }
        write ( fd, ( void * ) &erro, sizeof ( int ) );
        write ( fd, ( void * ) &addclientestruct->nome, sizeof ( char ) * 256 );
        close ( fd );
    }
}

int nomecheck ( global *info, char *cliente, cltusr *aux ) {
    int numint, len, fd, erro;
    cltusr *actual = info->listclientes;
    char numchar[3];

    while ( actual ) {
        if ( strcmp ( aux->nome, actual->nome ) == 0 ) {
            len = strlen ( aux->nome );
            //esta parte serve para identificar se os ultimos char sao numeros
            if (
                aux->nome[len - 3] >= '0' &&
                aux->nome[len - 3] <= '9' &&
                aux->nome[len - 2] >= '0' &&
                aux->nome[len - 2] <= '9' &&
                aux->nome[len - 1] >= '0' &&
                aux->nome[len - 1] <= '9' ) {
                numchar[0] = aux->nome[len - 3];
                numchar[1] = aux->nome[len - 2];
                numchar[2] = aux->nome[len - 1];
                numint = atoi ( numchar );
                if ( info->debug == 1 )
                    fprintf ( stderr, "aux->nome = %s; numint = %d\n", aux->nome, numint );
                if ( numint < 999 ) {
                    sprintf ( numchar, "%03d", ++numint );
                    if ( info->debug == 1 )
                    fprintf ( stderr, "numchar = %s\n", numchar );
                    aux->nome[len - 3] = numchar[0];
                    aux->nome[len - 2] = numchar[1];
                    aux->nome[len - 1] = numchar[2];
                }

                else {
                    fd = open ( cliente, O_WRONLY );
                    if ( fd == -1 ) {
                        kill ( aux->pid, SIGINT );
                    } else {
                        erro = INVALID_CLIENT_NAME;
                        write ( fd, ( void * ) &erro, sizeof ( int ) );
                        close ( fd );
                    }
                }
            } else {
                strcat ( aux->nome, "001" );
            }
        }
        actual = actual->prox;
    }
    return 0;
    
}

void intapagaclientes ( global *info ) {
    if ( info->listclientes != NULL ) {
        cltusr *aux = info->listclientes->prox, *auxant = info->listclientes;

        while ( aux != NULL ) {
            auxant = aux;
            aux = auxant->prox;
        }
        aux = auxant;
        while ( auxant != NULL ) {
            aux = auxant;
            auxant = auxant->ant;
            free ( aux );
        }
    }
}

void freethings ( global *info ) {
    printf ( "verificar o info --FREETHINGS-- \n" );
    printf ( "info->cpid %d", info->cpid );
    printf ( "end da verifcacao\n" );
    info->terminate = 1;
    pthread_cancel ( info->read_fifo );
    pthread_join ( info->read_fifo, NULL );
    if ( info != NULL ) {
        killverifica ( info->cpid );
        if ( info->listclientes != NULL ) {
            cltusr *auxc = info->listclientes, *proxauxc;
            proxauxc = auxc->prox;
            while ( auxc ) {
                kill ( auxc->pid, SIGINT );
                free ( auxc );
                auxc = proxauxc;
                if ( auxc )
                    proxauxc = auxc->prox;
            }
        }
        if ( info->listtopicos ) {
            tpc *auxt = info->listtopicos, *proxauxt;
            msg *auxm, *proxauxm;
            proxauxt = auxt->prox;
            while ( auxt ) {
                if ( auxt->primsg ) {
                    auxm = auxt->primsg;
                    proxauxm = auxm->prox;
                    while ( auxm ) {
                        free ( auxm );
                        auxm = proxauxm;
                        if ( auxm )
                            proxauxm = auxm->prox;
                    }
                }
                free ( auxt );
                auxt = proxauxt;
                if ( auxt )
                    proxauxt = auxt->prox;
            }
        }
        free ( info );
        info = NULL;
    }
    unlink ( GESTORFIFO );
    exit ( 0 );
}
