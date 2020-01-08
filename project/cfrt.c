
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
            wprintw(info->notification, "[AVISO]o gestor esta se a desligar, este cliente ira fechar dentro de 10 segundos.");
            wrefresh(info->notification);
            sleep(10);
            printf("o servidor encerrou.\n");
            pthread_cancel(info->threads);
        } else if (recebe.codigo == KICK) {
            wclear(info->notification);
            wprintw(info->notification, "[AVISO]foste kickado pelo admin, este cliente ira fechar dentro de 10 segundos.");
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


void terminar(global *info)
{
    char charpid[10];
    endwin();


    sprintf(charpid, "%d", getpid());
    unlink(charpid);
    pipemsg envrcb;
    int fd = open("gestor-fifo", O_WRONLY | O_NONBLOCK), bytes;
    if (fd > 1) {
        envrcb.codigo = CLOSING_CLIENT;
        envrcb.pid = getpid();
        printf("exiting...\n");
        bytes = write(fd, (void *) &envrcb.codigo, sizeof(pipemsg));
        if (info->debug == 1)
            printf("sent %d bytes with the value %d, pid = %d\n", bytes, envrcb.codigo, envrcb.pid);

        close(fd);
    } else {
        printf("problemas a abrir para mandar a mensagem de fechar.");
        if (info->debug == 1)
            printf("[ERRO=%d]\n", fd);
        else
            printf("\n");
    }

    exit(255);
}

msg sendmsg(global * info, char * msg, int tpcid){
    
}