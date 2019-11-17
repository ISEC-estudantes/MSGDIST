/*
Trabalho de SO 2019 ISEC

Trabalho feito:
  João Gonçalves 21280302
  João Lopes     21270423
*/

//GESTOR - MAIN

#include "utils.h"
#include "verificar.h"
#include "cmd.h"




void exitNow(int s) {
    exit(0);
};


int welcome(){
    printf(
 " __  __ ___  ___ ___ ___ ___ _____                     _            \n"
 "|  \\/  / __|/ __|   \\_ _/ __|_   _|  ___   __ _ ___ __| |_ ___ _ _  \n"
 "| |\\/| \\__ \\ (_ | |) | |\\__ \\ | |   |___| / _` / -_|_-<  _/ _ \\ '_| \n"
 "|_|  |_|___/\\___|___/___|___/ |_|         \\__, \\___/__/\\__\\___/_|   \n"
 "                                          |___/                     \n");
    return 0;
}

int fhelp(){
    welcome();
    printf("trabalho feito por:\n");
    return 0;
};


int main ( int argc, char **argv )
{
    welcome();
    
  //filtrar palavras proibidas on/off
  int filter=1, cmd=1, help = 0, rcv, env, maxnot, maxmsg, pid;

  getoption(argc, argv, &filter, &cmd, &help);

  if(help==1){
      fhelp();
      exit(0);
}
  
  
  char wordsnot[100];

  
  getvars ( &maxmsg, &maxnot, wordsnot );

  printf ( "maxmsg = %d\n"
           "maxnot = %d\n"
           "wordsnot = %s\n",
           maxmsg, maxnot, wordsnot );

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
   signal(SIGUSR2, exitNow);
  initverifica ( "verifica","proibidas", &rcv, &env, &pid );
  printf ( "result: %d\n",verificamsg ( env, rcv, "test not to test" ) );
  cmdlineprinc();
  killverifica ( pid );
  return 0;
}

