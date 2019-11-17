#ifndef VERIFICAR_H
#define VERIFICAR_H

//verifica - liga o verificador
//comando-nome do verificador;
//file_proividas-ficheiro com as palavras proibidas;
//rcv-pipe para receber informação do verificador;
//env-pipe para enviar informação ao verificador;
//swicher - 1:liga o verificador, 0:desliga o verificador //ou meter aqui o pid para desligar o verificador
int initverifica(char *comando, char *file_proibidas, int *rcv, int *env, int *pid);

//verifica uma mensagem msg com o verificador
int verificamsg(int pipein, int pipeout, char * msg);

//terimina o verificador
void killverifica(int pid);


//ve os erros que o verificador possa ter
int errosverificador(char *file_proibidas);
#endif
