/*
 * Trabalho de SO 2019 ISEC
 *
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 *
*/
//GESTOR - MAIN

#include "./headers/utils.h"
#include "./headers/verificar.h"
#include "./headers/gfrt.h"
#include "./headers/comand.h"

global * info = NULL;


void exitChild()
{

    int erro = 0;

    waitpid(info->cpid, &erro, 0);
    switch (WEXITSTATUS(erro)) {
    case 2:
        printf("\nO ficheiro das palavras proibidas não existe\n");
        exit(2);
        break;

    case 3:
        printf("\nNão existem palavras proibidas no ficheiro delas\n");
        exit(3);
        break;

    case 4:
        printf("\nO comando verificador nao existe\n");
        exit(4);
        break;
    }
}

void exitNow()
{
    if (info->debug == 1) {
        printf("verificar o info --EXITNOW-- \n");
        printf("info->cpid %d", info->cpid);
        printf("end da verifcacao\n");
    }
    freethings(info);
};

int welcome()
{
    printf(
        " __  __ ___  ___ ___ ___ ___ _____                     _            \n"
        "|  \\/  / __|/ __|   \\_ _/ __|_   _|  ___   __ _ ___ __| |_ ___ _ _  \n"
        "| |\\/| \\__ \\ (_ | |) | |\\__ \\ | |   |___| / _` / -_|_-<  _/ _ \\ '_| \n"
        "|_|  |_|___/\\___|___/___|___/ |_|         \\__, \\___/__/\\__\\___/_|   \n"
        "                                          |___/                     \n");
    return 0;
}

int fhelp()
{
    welcome();
    printf("trabalho feito por:\n"
           "\tJoão Gonçalves 21280302\n"
           "\tJoão Lopes     21270423\n");

    return 0;
};

int main(int argc, char **argv)
{
    //filtrar palavras proibidas on/off
    int filter = 0, ocmd = 1, help = 0, maxnot, maxmsg, erro, debug = 0, maxusers, maxtimeout;

    getoption(argc, argv, &filter, &ocmd, &help, &debug);

    if (help == 1) {
        fhelp();
        exit(0);
    }

    if (access(GESTORFIFO, R_OK) != -1) {
        printf("Um gestor já esta a correr neste diretorio.\n");
        return 5;
    }

    char wordsnot[100];

    getvars(&maxmsg, &maxnot, wordsnot, &maxtimeout, &maxusers);

    if (debug == 1)
        printf("maxmsg = %d\n"
               "maxnot = %d\n"
               "wordsnot = %s\n"
               "maxtimeout = %d\n"
               "maxusers = %d\n",
               maxmsg, maxnot, wordsnot, maxtimeout, maxusers);

    //////////////////////////////////////////////
    ///////////////////CRIACAO DO GLOBAL info/////

    info = initinfo();

    info->filter = filter;
    info->debug = debug;
    info->maxmsg = maxmsg;
    info->maxnot = maxnot;
    info->maxtimeout = maxtimeout;
    info->maxusers = maxusers;
    info->wordsnot = wordsnot;

    ////////////////////////////////////////////////
    ///////////////////tratamento de sinais/////////

    signal(SIGUSR2, exitNow);
    signal(SIGCHLD, exitChild);
    signal(SIGINT, exitNow);

    ////////////////////////////////////////////////
    //////////////CRIACAO DAS PIPES/////////////////

    erro = mkfifo(GESTORFIFO, 0666);
    if (erro == -1) {
        printf("erro a criar o fifo\n");
        return -2;
    }

    ///////////////////////////////////////////
    /////////////////////VERIFICADOR///////////
    if ((erro = initverifica("verificador", wordsnot, &(info->rcv), &(info->env), &(info->cpid))) != 0)
        return erro;

    ////////////////////////////////////////////////
    //////////criacao do fifo do gestor/////////////

    pthread_create(&(info->read_fifo), NULL, (void *)readingfifo, (void *)info);
    
    ////////////////////////////////////////////////
    //////////////////START TIMER///////////////////

    //pthread_create(&(info->timer), NULL, (void*)msgdeleter, (void*)info);

    /////////////////////////////////////////////////
    ////////////////////COMAND LINE//////////////////
    
    //msgdeleter(info);
    welcome();
    cmd(info);

    ///////////////////////////////////////
    /////////ENCERRAMENTO DE TUDO//////////

    freethings(info);

    return 0;
}
