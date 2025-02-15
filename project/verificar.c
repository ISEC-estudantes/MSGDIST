/*
 * Trabalho de SO 2019 ISEC
 * 
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 * 
*/
//GESTOR - VIRIFICADOR RELATED FUNCTIONS


#include "./headers/verificar.h"

//verifica - liga o verificador
//comando-nome do verificador;
//file_proividas-ficheiro com as palavras proibidas;
//rcv-pipe para receber informação do verificador;
//env-pipe para enviar informação ao verificador;
//swicher - 1:liga o verificador, 0:desliga o verificador //ou meter aqui o pid para desligar o verificador
int initverifica ( char *comando, char *file_proibidas, unsigned int *rcv, unsigned int *env, unsigned int *pid ) {

    int error = 0;
    //error = errosverificador ( file_proibidas );
    if ( error != 0 )
        return error;

    int p[2], pr[2];
    if ( pipe ( p ) !=0 ) return -2;
    if ( pipe ( pr ) !=0 ) return -2;

    if ( ( *pid = fork() ) == 0 ) {
        //child process
        close ( 0 ); //fecha acesso ao teclado
        dup ( p[0] ); //duplicar p[0] na primeira posicao disponivel
        close ( p[0] ); //fechar extrem
        //idade de leitura do pipe - p[0]
        close ( p[1] ); //fechar extremidade de leitura do pipe - p[1]


        close ( 1 );  //fechar o acesso ao stdout
        dup ( pr[1] ); //meter la o pipe para escrever
        close ( pr[0] ); //fechar extremidade de leitura do pipe - p[0]
        close ( pr[1] ); //fechar extremidade de leitura do pipe - p[1]

        execl ( comando, comando, file_proibidas, NULL );
        fprintf ( stderr,"nao consegui executar o verificador\n" );
        exit ( 4 ); //fechar
    }

    if ( *pid < 0 ) return -1;

    close ( p[0] );
    close ( pr[1] );

    *env = p[1];
    *rcv = pr[0];

    //printf("%s \n", message);
    return 0;
}



int verificamsg ( int env, int rcv, char * msg ) {
    char response[10];
    char  msgend[] = " ##MSGEND## \n" ;
    int sizeofall = strlen ( msg ) + strlen ( msgend );

    char envia[sizeofall] ;
    strcpy ( envia, msg );
    strcat ( envia,msgend );
    //fprintf ( stdout,"msg = %s\n", msg );

    //escreve na pipe para enviar informação ao verificador
    write ( env, envia, strlen ( envia ) );

    //recebe informacao
    int n = read ( rcv, response, sizeof ( response ) );

    response[n-1] ='\0';

    //fprintf ( stdout,"response = %s[%d]\n", response, n );
    return atoi ( response );

}

void killverifica ( int pid ) { 
    kill(pid, SIGUSR2);
    wait ( &pid );
}


