#include "utils.h"


void insere()
{
  printf ( "\n" );
  printf ( "👉 " );
  fflush ( stdout );
}



int getvars ( int *maxmsg, int *maxnot, char * wordsnot )
{
  char *result;
  
  result = getenv ( "MAXMSG" );
  if ( ( result == NULL ) || result < 0 )
    *maxmsg = DEF_MAXMSG;
  else
    *maxmsg = atoi ( result );


  result = getenv ( "MAXNOT" );
  if ( ( result == NULL ) || result < 0 )
    *maxnot = DEF_MAXNOT;
  else
    *maxnot = atoi ( result );

  result = getenv ( "WORDSNOT" );

  if ( result == NULL )
    strcpy ( wordsnot, DEF_WORDSNOT );
  else
    strcpy ( wordsnot, result );

  return 0;
}

//funcao para ir buscar as opcoes
int getoption ( int argc, char **argv, int *filter, int *cmd,int * help, int * debug )
{

  int opcao;  // Opção passada pelo usuário ao programa.

  // Desativa as mensagens de erro da função getopt
  //opterr = 0;

  // Faz um loop pegando as opções passados pelo usuário. Note
  // o ':' depois do 'z'. Isso quer dizer que deve haver um
  // argumento depois dessa opção.
  while ( ( opcao = getopt ( argc, argv, "fch" ) ) != -1 )
    {
      switch ( opcao )
        {
        //desactivar o filtro
        case 'f':
          *filter = 1;
          break;
        //desactivar o cmdline
        case 'c':
          *cmd = 1;
          break;
        //mostrar o help menu
        case 'h':
          *help=1;
          break;
        //mostrar coisas de debug
        case 'd':
            *debug = 1;
        break;
        }
    }
  return 0;
}

