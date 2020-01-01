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

int getvars(int *maxmsg, int *maxnot, char *wordsnot, int *maxtimeout, int *maxusers, int *maxtopics)
{
    char *resultchar;
    int resultint;

    resultchar = getenv("MAXMSG");
    if (resultchar == NULL)
        *maxmsg = DEF_MAXMSG;
    else if ((resultint = atoi(resultchar)) < 0)
        * maxmsg = resultint;
    else
        *maxmsg = DEF_MAXMSG;


    resultchar = getenv("MAXNOT");
    if (resultchar == NULL)
        *maxnot = DEF_MAXNOT;
    else if ((resultint = atoi(resultchar)) < 0)
        * maxnot = resultint;
    else
        *maxmsg = DEF_MAXNOT;

    resultchar = getenv("WORDSNOT");
    if (resultchar == NULL)
        strcpy(wordsnot, DEF_WORDSNOT);
    else
        strcpy(wordsnot, resultchar);

    resultchar = getenv("MAXTIMEOUT");
    if (resultchar == NULL)
        *maxtimeout = DEF_MAXTIMEOUT;
    else if ((resultint = atoi(resultchar)) < 0)
        * maxtimeout = resultint;
    else
        *maxtimeout = DEF_MAXTIMEOUT;


    resultchar = getenv("MAXUSERS");
    if (resultchar == NULL)
        *maxusers = DEF_MAXUSERS;
    else if ((resultint = atoi(resultchar)) < 0)
        * maxusers = resultint;
    else 
        *maxusers = DEF_MAXUSERS;
    
    resultchar = getenv("MAXTOPICS");
    if (resultchar == NULL)
        *maxtopics = DEF_MAXTOPICS;
    else if ((resultint = atoi(resultchar)) < 0)
        *maxtopics = DEF_MAXTOPICS;
    else 
        *maxtopics = resultint;

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
    global *info = (global *)malloc(sizeof(global));
    if (info == NULL) {
        fprintf(stderr, "problemas a allocar memoria ma boy.\n");
        exit(3);
    }
    info->nclientes = info->ntopicos = 0;
    info->listclientes = NULL;
    info->listtopicos = NULL;
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

pipemsg initpipemsg()
{
    pipemsg a;
    a.codigo = 0;
    a.topicid = 0;
    a.msgid = 0;
    a.pid = 0;
    return a;
}



