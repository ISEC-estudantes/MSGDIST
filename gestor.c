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

void exitChild()
{
     int erro = 0;
     

     waitpid ( cpid,&erro, 0 );

     if ( erro == 2 ) {
          printf ( "O ficheiro %s nao existe\n", wordsnot );
          exit ( 2 );
     }else

     if ( erro == 3 ) {
          printf ( "Não existem palavras proibidas no ficheiro \"%s\"\n", wordsnot );
          exit ( 3 );
     }else
     printf("Não o comando nao é verificador nao existe\n");

    
}

void exitNow ()
{
    
    killverifica ( cpid );
    exit ( 0 );
};


int welcome()
{
     printf (
          " __  __ ___  ___ ___ ___ ___ _____                     _            \n"
          "|  \\/  / __|/ __|   \\_ _/ __|_   _|  ___   __ _ ___ __| |_ ___ _ _  \n"
          "| |\\/| \\__ \\ (_ | |) | |\\__ \\ | |   |___| / _` / -_|_-<  _/ _ \\ '_| \n"
          "|_|  |_|___/\\___|___/___|___/ |_|         \\__, \\___/__/\\__\\___/_|   \n"
          "                                          |___/                     \n" );
     return 0;
}

int fhelp()
{
     welcome();
     printf ( "trabalho feito por:\n" );
     return 0;
};


int main ( int argc, char **argv )
{    cpid = 0;
    
     welcome();

     //filtrar palavras proibidas on/off
     int filter=1, ocmd=1, help = 0, rcv, env, maxnot, maxmsg, erro, debug =0, maxusers, maxtimeout;

     getoption ( argc, argv, &filter, &ocmd, &help, &debug );

     if ( help==1 ) {
          fhelp();
          exit ( 0 );
     }


     char wordsnot[100];


     getvars ( &maxmsg, &maxnot, wordsnot , &maxtimeout, &maxusers);

     if ( debug == 1 )
          printf ( "maxmsg = %d\n"
                   "maxnot = %d\n"
                   "wordsnot = %s\n",
                   "maxtimeout = %d\n",
                   "maxusers = %d\n",
                   maxmsg, maxnot, wordsnot , maxtimeout, maxusers);

     /*

     printf("filter is %d\n", filter);
     printf("cmd is %d\n", cmd);

     unsigned int maxmsg = getmaxmsg();
     if (maxmsg == -1){
      printf("Não existe um limite de mensagens, por favor ensira um com uma variavel de ambiente com o nome MAXMSG.\n");
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
     if ( ( erro = initverifica ( "verificador",wordsnot, &rcv, &env, &cpid ) ) !=0 )
          return erro;
     printf ( "result: %d\n",verificamsg ( env, rcv, "test not to test" ) );


     char raw[255];
     char *cmd[10] = {};
     int ncmd = 0;
     printf ( "Bem vindo a linha de comandos do gestor do msgdist,\n insira help ou h para pedir ajuda e dicas.\n" );
     insere();
     while ( fgets ( raw,200,stdin ) ) {
          if ( raw[0]!='\n') {
               
               raw[strlen ( raw ) - 1] = '\0'; //trocar o \n por \0 para dizer o limite da cmd
               cmd[ncmd] = strtok ( raw, " " );

               while ( ( cmd[++ncmd]= strtok ( NULL," " ) ) != NULL );
               /*
               for ( int i = 0; i< ncmd; i++ )
                 {
                   printf ( "\nncmd[%d] = %s \n", i, ncmd[i] );
                 }
                 */
               if ( strcmp ( cmd[0], "shutdown" ) ==0 || strcmp ( cmd[0], "s" ) ==0 ) {
                    printf ( "\tight imma head out\n" );
                    exitNow();
                    return 0;
               } else if ( strcmp ( cmd[0], "help" ) ==0 || strcmp ( cmd[0], "h" ) ==0 ) {
                    printf ( "    Comandos disponiveis:\n"
                             "help ou h - - - - - - - este ecra\n"
                             "shutdown ou s - - - - - terminar o sistema MSGDIST\n" );

               } else
                    //outros comandos
                    printf ( "Comando \"%s\" não encontrado.\n",cmd[0]);

          }
          insere();
     }

     killverifica ( cpid );
     return 0;
}

