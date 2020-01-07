/*
 * Trabalho de SO 2019 ISEC
 *
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 *
*/
//GESTOR - COMAND LINE THREAD FUNCTIONS


#include "./headers/comand.h"
#include "./headers/gfrt.h"

void *cmd(void *input)
{
    global *info = (global *)input;
    cltusr *aux;
    char raw[255], copia[255];
    char *cmd[10] = {}, *endptr;
    int ncmd = 0, erro = 0, counter;
    long int val ;
    pipemsg env;
    printf("Bem vindo a linha de comandos do gestor do msgdist,\n insira help ou h para pedir ajuda e dicas.\n");
    insere();
    while (fgets(raw, 200, stdin)) {
        ncmd = 0;
        strcpy(copia, raw);
        if (raw[0] != '\n') {
            cmd[ncmd] = strtok(raw, " \n\t");

            while ((cmd[++ncmd] = strtok(NULL, " \n\t")) != NULL)
                ;

            if (info->debug == 1) {
                for (int i = 0; i < ncmd; i++)
                    printf("\nncmd[%d] = %s \n", i, cmd[i]);
            }

            if (strcmp(cmd[0], "shutdown") == 0 || strcmp(cmd[0], "s") == 0) {
                printf("\tight imma head out\n");
                return 0;
            } else if (strcmp(cmd[0], "help") == 0 || strcmp(cmd[0], "h") == 0) {
                printf("    Comandos disponiveis:\n"
                       "help ou h - - - - - - - -  este ecra\n"
                       "shutdown ou s - - - - - -  terminar o sistema MSGDIST\n"
                       "test-verificador ou tv  -  testa uma frase no verificador\n"
                      );
            } else if (strcmp(cmd[0], "tv") == 0 || strcmp(cmd[0], "test-verificador") == 0) {
                if (ncmd < 2) {
                    printf("insira a frase que quer enviar > ");
                    fflush(stdout);
                    fgets(raw, 200, stdin);
                    write(info->env, raw, strlen(raw));
                    write(info->env, " ##MSGEND## \n", 14);
                    erro = read(info->rcv, raw, 4);
                    raw[erro - 1] = '\0';
                    printf("tem %s palavras proibidas na sua frase\n", raw);
                } else {
                    for (erro = strlen(cmd[0]); erro < strlen(copia); ++erro)
                        write(info->env, &copia[erro], 1);
                    write(info->env, " ##MSGEND## \n", 14);
                    erro = read(info->rcv, raw, 4);
                    raw[erro - 1] = '\0';
                    printf("tem %s palavras proibidas na sua frase\n", raw);
                }
            } else if (strcmp(cmd[0], "filter") == 0 || strcmp(cmd[0], "f") == 0) {
                if (ncmd > 2) {
                    printf("tem argumentos a mais");
                }
                if (ncmd == 2) {
                    if (strcmp(cmd[1], "on")) {
                        info->filter = 1;
                    }
                    if (strcmp(cmd[1], "off")) {
                        info->filter = 0;
                    }
                }
            } else if (strcmp(cmd[0], "users") == 0 || strcmp(cmd[0], "us") == 0) {
                if (info->listclientes) {
                    counter = 0;
                    aux = info->listclientes;
                    printf("Users conectados:\n");
                    printf("PID     NOME\n");
                    while (aux) {
                        printf("\n%d %s\n", aux->pid, aux->nome);
                        ++counter;
                        aux = aux->prox;
                    }
                    printf("Existem %d users online.\n", counter);
                } else {
                    printf("Não existem utilizadores online.\n");
                }
            } else if (strcmp(cmd[0], "topics") == 0 || strcmp(cmd[0], "ts") == 0) {
                printf("\tEste comando ainda nao esta implementado.\n");
            } else if (strcmp(cmd[0], "msg") == 0) {
                printf("\tEste comando ainda nao esta implementado.\n");
            } else if (strcmp(cmd[0], "topic") == 0) {
                printf("\tEste comando ainda nao esta implementado.\n");
            } else if (strcmp(cmd[0], "del") == 0) {
                printf("\tEste comando ainda nao esta implementado.\n");
            } else if (strcmp(cmd[0], "kick") == 0) {
                if (ncmd == 2) {
                    errno = 0;    /* To distinguish success/failure after call */
                    val = strtol(cmd[1], &endptr, 10);

                    /* Check for various possible errors */

                    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
                            || (errno != 0 && val == 0)) {
                        errokick();
                    } else if (endptr == cmd[1]) {
                        errokick();
                    }else if(val > INT_MAX){
                        errokick();  
                    }else{
                    removebypid(info , val);
                    val = open(cmd[1], O_WRONLY | O_NONBLOCK );
                    env.codigo = KICK;
                    write(val, (void *)&env, sizeof(pipemsg));
                    close(val);
                    
                    }
                } else {
                    errokick();
                }
            } else if (strcmp(cmd[0], "prune") == 0) {
                printf("\tEste comando ainda nao esta implementado.\n");
            } else
                //outros comandos
                printf("Comando \"%s\" não encontrado.\n", cmd[0]);
        }

        insere();
    }
    return 0;
}

void errokick()
{
    printf("Use o seguinte formato:\n"
           "kick piddocliente\n"
           "pode obter o pid dos clientes com o comando us ou users\n"
          );
}
