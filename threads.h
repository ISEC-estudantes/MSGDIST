#ifndef THREADS_H
#define THREADS_H

#include "utils.h"

void readingfifo(void * input);

void addcliente(void* received);
void freethings(global * info);

int nomecheck(global *info, char *cliente, cltusr *aux);
#endif //THREADS_H