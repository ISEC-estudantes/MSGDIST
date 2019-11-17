#include "cmd.h"

/*
int cmdline()
{
 int i, len;
 char comando[100];
 for (;;)
 {
   //system("clear");
   fflush(stdin);
   fgets(comando, 100, stdin);
   comando[strlen(comando) - 1] = '\0';
   printf("test: %s\n", comando);
   len = strlen(comando);
   //for(i=0;i<len;++i){
   //limpar espaços e enters e tabs entre outros
   //}
   if (strcmp(comando, "fim") == 0)
   {
     return 0;
   }
 }
 return 0;
}
int cmdline2()
{
 char str[] = "hoje e mes novembro";
 char *pal[10];
 int n = 0;
 pal[n] = strtok(str, " ");
 while (pal[n] != NULL)
 {
   pal[++n] = strtok(NULL, " ");
 }
 for (int i = 0; i < n; i++)
 {

 }
 return 0;
}
*/

int cmdlineprinc()
{
  char cmd[255];
  char *pal[20];
  int n = 0;

  for ( ;; )
    {
      printf ( "👉 " );
      fflush ( stdout );
      fgets ( cmd,200,stdin );
      if ( cmd[0]!='\n' )
        {
          cmd[strlen ( cmd ) - 1] = '\0'; //trocar o \n por \0 para dizer o limite da cmding
          pal[n] = strtok ( cmd, " " );
          while ( pal[n] != NULL )
            {
              pal[++n] = strtok ( NULL, " " );
            }
          for ( int i = 0; i< n; i++ )
            {
              printf ( "\npal[%d] = %s \n", i, pal[i] );
            }
          if ( strcmp ( pal[0],"shutdown" ) ==0 )
            {
              printf ( "ight imma head out\n" );
              //  return 0;

            }
        }
    }

  return 0;

}
