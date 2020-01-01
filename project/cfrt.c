
/*
 * Trabalho de SO 2019 ISEC
 *
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 *
*/
//CLIENTE - VIRIFICADOR RELATED FUNCTIONS

#include "./headers/cfrt.h"

void readingfifo(void *input)
{
    //pthread_t handler_creat;
    global *info = (global *) input;
    char myfifochar[10];
    sprintf(myfifochar, "%d", info->pid);
    int myfifo = info->fifo_cliente, bytes;
    pipemsg recebe = initpipemsg();

    while (info->terminate != 1) {
        if (info->debug == 1) {
            wclear(info->notification);
            wprintw(info->notification, "[THREAD_READINGFIFO] new round\n");
            wrefresh(info->notification);
        }

        bytes = read(myfifo, (void *) &recebe, sizeof(pipemsg));
        if (info->debug == 1) {
            wclear(info->notification);
            wprintw(info->notification, "[THREAD_READINGFIFO] bytes recebidos =%d, codigo = %d, pid = %d \n", bytes, recebe.codigo, recebe.pid);
            wrefresh(info->notification);
        }

        if (recebe.codigo == SHUTDOWN) {
            wclear(info->notification);
            wprintw(info->notification, "o gestor esta se a desligar, este cliente ira fechar dentro de 10 segundos.");
            printf("o servidor encerrou.\n");
            wrefresh(info->notification);
            sleep(10);
            pthread_cancel(info->threads);
        } else if (recebe.codigo == KICK) {
            wclear(info->notification);
            wprintw(info->notification, "foste kickado pelo admin, este cliente ira fechar dentro de 10 segundos.");
            printf("foste kickado pelo admin.\n");
            wrefresh(info->notification);
            sleep(10);
            pthread_cancel(info->threads);
        } else if (recebe.codigo == ADD_MESSAGE) {
            //A IMPLEMENTAR
        } else if (recebe.codigo == NOTIFICATION) {
            //A IMPLEMENTAR
        }
    }
    close(myfifo);
}



