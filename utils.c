#include "utils.h"


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


