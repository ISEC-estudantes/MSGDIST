#include "cmd.h"

int cmdlineprinc()
{
  char raw[255];
  char *argv[10];
  int argc = 0;
    printf("Bem vindo a linha de comandos do gestor do msgdist,\n insira help ou h para pedir ajuda e dicas.\n");
  printf ( "\n" );
  printf ( "👉 " );
  fflush ( stdout );
  while ( fgets ( raw,200,stdin ) )
    {
      if ( raw[0]!='\n' )
        {
          argc=0;
          raw[strlen ( raw ) - 1] = '\0'; //trocar o \n por \0 para dizer o limite da cmd
          argv[argc] = strtok ( raw, " " );
          while ( argv[argc] != NULL )
            {
              argv[++argc] = strtok ( NULL, " " );
            }
          /*
          for ( int i = 0; i< argc; i++ )
            {
              printf ( "\nargv[%d] = %s \n", i, argv[i] );
            }
            */
          if ( strcmp ( argv[0], "shutdown" ) ==0 || strcmp ( argv[0], "s" ) ==0  )
            {
              printf ( "\tight imma head out\n" );
              return 0;
            } else if (strcmp ( argv[0], "help" ) ==0 || strcmp ( argv[0], "h" ) ==0 ){
                printf("    Comandos disponiveis:\n"
                    "help ou h - - - - - - - este ecra\n"
                    "shutdown ou s - - - - - terminar o sistema MSGDIST\n");
                
            }
          else
            //outros comandos
            printf ( "Comando \"%s\" não encontrado.\n",argv[0] );

        }
      printf ( "\n" );
      printf ( "👉 " );
      fflush ( stdout );
    }

  return 0;
}

