/*
 * Trabalho de SO 2019 ISEC
 *
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 *
*/
//ALL - GENERIC INFORMATION

#include "./headers/utils.h"

void insere()
{
    printf("\n");
    printf("👉 ");
    fflush(stdout);
}



int getvars(int *maxmsg, int *maxnot, char *wordsnot, int *maxtimeout, int *maxusers)
{
    char *resultchar;
    int resultint;

    resultchar = getenv("MAXMSG");
    resultint = atoi(resultchar);
    if ((resultchar == NULL) || resultint < 0)
        *maxmsg = DEF_MAXMSG;
    else
        *maxmsg = resultint;


    resultchar = getenv("MAXNOT");
    resultint = atoi(resultchar);
    if ((resultchar == NULL) || resultint < 0)
        *maxnot = DEF_MAXNOT;
    else
        *maxnot = resultint;

    resultchar = getenv("WORDSNOT");
    if (resultchar == NULL)
        strcpy(wordsnot, DEF_WORDSNOT);
    else
        strcpy(wordsnot, resultchar);

    resultchar = getenv("MAXTIMEOUT");
    resultint = atoi(resultchar);
    if ((resultchar == NULL) || resultint < 0)
        *maxtimeout = DEF_MAXTIMEOUT;
    else
        *maxtimeout = resultint;


    resultchar = getenv("MAXUSERS");
    resultint = atoi(resultchar);
    if ((resultchar == NULL) || resultint < 0)
        *maxusers = DEF_MAXUSERS;
    else
        *maxusers = resultint;


    return 0;
}

//funcao para ir buscar as opcoes
int getoption(int argc, char **argv, int *filter, int *cmd, int *help, int *debug)
{

    int opcao;  // Opção passada pelo usuário ao programa.

    // Desativa as mensagens de erro da função getopt
    //opterr = 0;

    // Faz um loop pegando as opções passados pelo usuário. Note
    // o ':' depois do 'z'. Isso quer dizer que deve haver um
    // argumento depois dessa opção.
    while ((opcao = getopt(argc, argv, "fchd")) != -1) {
        switch (opcao) {
        //desactivar o filtro
        case 'f':
            *filter = 1;
            break;
        //desactivar o cmdline
        case 'c':
            *cmd = 0;
            break;
        //mostrar o help menu
        case 'h':
            *help = 1;
            break;
        //mostrar coisas de debug
        case 'd':
            *debug = 1;
            break;
        }
    }
    return 0;
}

void semmem()
{
    fprintf(stderr, "O dispositivo esta' sem memoria.\n");
}

global *initinfo()
{

    global * info = (global *)malloc(sizeof(global));
    if (info == NULL) {
        fprintf(stderr,"problemas a allocar memoria ma boy.\n");
        exit(3);
    }
    info->nclientes = info->ntopicos = 0;
    info->listclientes = info->lastclient = NULL;
    info->listtopicos = info->lasttopic = NULL;
    info->debug = 0;
    info->maxmsg = 0;
    info->maxnot = 0;
    info->maxtimeout = 0;
    info->maxusers = 0;
    info->wordsnot = NULL;
    info->cpid = 0;
    info->terminate = 0;
    info->filter = 0;
    pthread_mutex_init(&info->lock_cltusr, NULL);
    pthread_mutex_init(&info->lock_tpc, NULL);
    
    return info;
}
