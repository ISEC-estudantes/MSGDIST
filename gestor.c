/*
Trabalho de SO 2019 ISEC

Trabalho feito:
  João Gonçalves 21280302
  João Lopes     21270423
*/

//GESTOR - MAIN

#include "utils.h"
#include "verificar.h"

int cpid;
char * wordsnot;

void exitChild() {
    
    int erro = 0;
    
    waitpid ( cpid,&erro, 0 );
    //printf("\nWIFEXITED ( erro ) = %d\n",WIFEXITED ( erro ) );
    //printf("\nWEXITSTATUS( erro ) = %d\n", WEXITSTATUS( erro ));
    switch ( WEXITSTATUS ( erro ) ) {
    case 2:
        printf ( "\nO ficheiro das palavras proibidas não existe\n" );
        exit ( 2 );
        break;

    case 3:
        printf ( "\nNão existem palavras proibidas no ficheiro delas\n");
        exit ( 3 );
        break;

    case 4 :
        printf ( "\nO comando verificador nao existe\n" );
        exit ( 4 );
        break;

    }

}

void exitNow () {

    killverifica ( cpid );
    exit ( 0 );
};


int welcome() {
    printf (
        " __  __ ___  ___ ___ ___ ___ _____                     _            \n"
        "|  \\/  / __|/ __|   \\_ _/ __|_   _|  ___   __ _ ___ __| |_ ___ _ _  \n"
        "| |\\/| \\__ \\ (_ | |) | |\\__ \\ | |   |___| / _` / -_|_-<  _/ _ \\ '_| \n"
        "|_|  |_|___/\\___|___/___|___/ |_|         \\__, \\___/__/\\__\\___/_|   \n"
        "                                          |___/                     \n" );
    return 0;
}

int fhelp() {
    welcome();
    printf ( "trabalho feito por:\n" );
    return 0;
};


int main ( int argc, char **argv ) {
    cpid = 0;

    welcome();

    //filtrar palavras proibidas on/off
    int filter=1, ocmd=1, help = 0, rcv, env, maxnot, maxmsg, erro, debug =0, maxusers, maxtimeout;

    getoption ( argc, argv, &filter, &ocmd, &help, &debug );

    if ( help==1 ) {
        fhelp();
        exit ( 0 );
    }


    char wordsnot[100];


    getvars ( &maxmsg, &maxnot, wordsnot, &maxtimeout, &maxusers );

    if ( debug == 1 )
        printf ( "maxmsg = %d\n"
                 "maxnot = %d\n"
                 "wordsnot = %s\n"
                 "maxtimeout = %d\n"
                 "maxusers = %d\n",
                 maxmsg, maxnot, wordsnot, maxtimeout, maxusers );

    /*

    printf("filter is %d\n", filter);
    printf("cmd is %d\n", cmd);

    unsigned int maxmsg = getmaxmsg();
    if (maxmsg == -1){
     printf("Não existe um limite de mensagens, por favor ensira um com uma variavel de ambiente com o nome MAXMSG.\n");`
     return 1;
    }
    if (maxmsg == -2){
     printf("Numero negativo MAXMSG, porfavor insira um numero positivo.\n");
     return 1;
    }

    printf("o numero é %d.\n",maxmsg);
    */
    signal ( SIGUSR2, exitNow );
    signal ( SIGCHLD, exitChild );
    if ( ( erro = initverifica ( "verificador",wordsnot, &rcv, &env, &cpid ) ) !=0 )  return erro;

    char raw[255], copia[255];
    char *cmd[10] = {};
    int ncmd = 0;

    printf ( "Bem vindo a linha de comandos do gestor do msgdist,\n insira help ou h para pedir ajuda e dicas.\n" );
    insere();
    while ( fgets ( raw,200,stdin ) ) {
        ncmd = 0;
        strcpy ( copia, raw );
        if ( raw[0]!='\n' ) {

            //raw[strlen ( raw ) - 1] = '\0'; //trocar o \n por \0 para dizer o limite da cmd
            cmd[ncmd] = strtok ( raw, " \n\t" );

            while ( ( cmd[++ncmd]= strtok ( NULL," \n\t" ) ) != NULL );

            if ( debug==1 ) {
                for ( int i = 0; i< ncmd; i++ )
                    printf ( "\nncmd[%d] = %s \n", i, cmd[i] );
            }

            if ( strcmp ( cmd[0], "shutdown" ) ==0 || strcmp ( cmd[0], "s" ) ==0 ) {
                printf ( "\tight imma head out\n" );
                exitNow();
                return 0;
            } else if ( strcmp ( cmd[0], "help" ) ==0 || strcmp ( cmd[0], "h" ) ==0 ) {
                printf ( "    Comandos disponiveis:\n"
                         "help ou h - - - - - - - -  este ecra\n"
                         "shutdown ou s - - - - - -  terminar o sistema MSGDIST\n"
                         "test-verificador ou tv  -  testa uma frase no verificador\n" );
            } else if ( strcmp ( cmd[0], "tv" ) ==0 || strcmp ( cmd[0], "test-verificador" ) ==0 ) {
                if ( ncmd<2 ) {
                    printf ( "insira a frase que quer enviar > " );
                    fflush ( stdout );
                    fgets ( raw, 200, stdin );
                    write ( env, raw, strlen ( raw ) );
                    write ( env, " ##MSGEND## \n", 14 );
                    erro = read ( rcv, raw, 4 );
                    raw[erro-1] = '\0';
                    printf ( "tem %s palavras proibidas na sua frase\n",raw );
                } else {
                    for ( erro = strlen ( cmd[0] ); erro < strlen ( copia ); ++erro ) write ( env, &copia[erro], 1 );
                    write ( env, " ##MSGEND## \n", 14 );
                    erro = read ( rcv, raw, 4 );
                    raw[erro-1] = '\0';
                    printf ( "tem %s palavras proibidas na sua frase\n",raw );

                }

            } else if ( strcmp ( cmd[0], "filter" ) ==0 || strcmp ( cmd[0], "f" ) ==0 ) {
                printf ( "\tEste comando ainda nao esta implementado.\n" );
            } else  if ( strcmp ( cmd[0], "users" ) ==0 || strcmp ( cmd[0], "us" ) ==0 ) {
                printf ( "\tEste comando ainda nao esta implementado.\n" );
            } else  if ( strcmp ( cmd[0], "topics" ) ==0 || strcmp ( cmd[0], "ts" ) ==0 ) {
                printf ( "\tEste comando ainda nao esta implementado.\n" );
            } else  if ( strcmp ( cmd[0], "msg" ) ==0  ) {
                printf ( "\tEste comando ainda nao esta implementado.\n" );
            } else  if ( strcmp ( cmd[0], "topic" ) ==0 ) {
                printf ( "\tEste comando ainda nao esta implementado.\n" );
            } else  if ( strcmp ( cmd[0], "del" ) ==0 ) {
                printf ( "\tEste comando ainda nao esta implementado.\n" );
            } else if ( strcmp ( cmd[0], "kick" ) ==0 ) {
                printf ( "\tEste comando ainda nao esta implementado.\n" );
            } else if ( strcmp ( cmd[0], "prune" ) ==0 ) {
                printf ( "\tEste comando ainda nao esta implementado.\n" );
            } else 
                //outros comandos
                printf ( "Comando \"%s\" não encontrado.\n",cmd[0] );
        }

        insere();
    }

    killverifica ( cpid );
    return 0;
}

