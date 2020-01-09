#ifndef COMAND_H
#define COMAND_H

/*
 * Trabalho de SO 2019 ISEC
 * 
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 * 
*/
//GESTOR - COMAND LINE THREAD FUNCTIONS

#include "utils.h"

void * cmd ( void *input);

void errokick();

void listmsgs(global * info);

#endif //COMAND_H
