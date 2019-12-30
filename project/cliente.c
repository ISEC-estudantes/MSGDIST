/*
 * Trabalho de SO 2019 ISEC
 *
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 *
*/
//CLIENTE - MAIN


#include "./headers/utils.h"

global *info = NULL;

void exitNow()
{
    char charpid[10];
    endwin();


    sprintf(charpid, "%d", getpid());
    unlink(charpid);
    pipemsg envrcb;
    int fd = open("gestor-fifo", O_WRONLY), bytes;
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

void welcome()
{
    printf(
        " __  __ ___  ___ ___ ___ ___ _____            _ _         _       \n"
        "|  \\/  / __|/ __|   \\_ _/ __|_   _|  ___   __| (_)___ _ _| |_ ___ \n"
        "| |\\/| \\__ \\ (_ | |) | |\\__ \\ | |   |___| / _| | / -_) ' \\  _/ -_)\n"
        "|_|  |_|___/\\___|___/___|___/ |_|         \\__|_|_\\___|_||_\\__\\___|\n");
}

void fhelp()
{
}

int main(int argc, char **argv)
{

    welcome();
    /*  - - -  - - - - - - - - - - - - - - - -
        VERIFICADOR DO USER
    */
    //a minha implementaçao de um verificador de opcoes e valores nos argumento
    cltusr myinfo;
    myinfo.prox = NULL;
    char pidchar[10];
    myinfo.pid = getpid();
    sprintf(pidchar, "%d", myinfo.pid);
    int i, ff_cliente, ff_gestor, debug = 0,  help = 0, error, fck, ff_lixo, bytes;


    error = getoption(argc, argv, &fck, &error, &help, &debug);
    if (debug == 1) {
        printf("MAIN-OPTIONS"
               "fck = %d\n"
               "help = %d\n"
               "debug = %d\n"
               , fck, help, debug
              );
    }
    info = initinfo();
    info->debug = debug;


    if (2 > argc) {
        printf("digite o seu nome: ");
        fflush(stdout);
        scanf("%99[^\n]", myinfo.nome);
    } else {
        strcpy(myinfo.nome, argv[1]);
        for (i = 2; i < argc; ++i) {
            strcat(myinfo.nome, " ");
            strcat(myinfo.nome, argv[i]);
        }
    }
    if (fck == 1) {
        ///////////////////////////////////////////////////////
        ///////////////COMUNICACAO COM O GESTOR////////////////


        if (mkfifo(pidchar, 0666) != 0) {
            printf("Unable to create a fifo");
            exit(-1);
        }
        ff_cliente = open(pidchar, O_RDWR);
        ff_cliente = open(pidchar, O_RDONLY);
        ff_lixo = open(pidchar, O_WRONLY);
        if (info->debug == 1)
            fprintf(stdout, "fifo criado com sucesso\n");

        //criar o pipemsg//

        pipemsg envrcb = initpipemsg();

        envrcb.codigo = ADD_CLIENT;
        strcpy(envrcb.clientname, myinfo.nome);
        envrcb.pid = myinfo.pid;


        //escreve para o fifo servidor//

        ff_gestor = open("gestor-fifo", O_WRONLY);
        if (ff_gestor > -1) {
            bytes = write(ff_gestor, (void *) &ff_gestor, sizeof(pipemsg));
            if (info->debug == 1)
                printf("sent %d bytes with the value %d, pid = %d\n", bytes, envrcb.codigo, envrcb.pid);

        } else {
            printf("O gestor nao esta em execucao.\n");
            unlink(pidchar);
            return 2;
        }


        //aguardar resposta

        read(ff_cliente, &envrcb, sizeof(pipemsg));
        if (envrcb.codigo == 0) {
            printf("Connectado ao gestor, a iniciar ncurses...\n");
            strcpy(myinfo.nome, envrcb.clientname);
        } else if (envrcb.codigo == INVALID_CLIENT_MAX) {
            printf("Maximo de clientes ligados ao gestor atingido, porfavor tente mais tarde.\n");
            unlink(pidchar);
            return 2;
        } else if (envrcb.codigo == INVALID_CLIENT_NAME) {
            printf("Existem 1000 pessoas com o mesmo nome do que tu, porfavor tenta algo mais original.\n");
            unlink(pidchar);
            return 2;
        } else {
            printf("Problemas no gestor, a desligar cliente.\n");
            int temp = i;
            char output[4];
            strcpy(output, ((char *) &temp));
            output[3] = '\0';

            printf("MENSAGEM\n\tint = %d\n\t char = %s\n", i, output);
            unlink(pidchar);
            return 2;
        }
    }
    printf("O seu nome é %s.", myinfo.nome);
    /* - -  - - - - - - -  - - - - --  --  --  --  - - - - - - - - -/
    /                    EDITOR DE TEXTO                            /
    /- - - - - -  - - - - - - - - - - - - - - - - - - - - --  -- - */

    initscr();

////////////////TRATAMENTO DE SINAIS///////////////////
    signal(SIGINT, exitNow);
    signal(SIGPIPE, exitNow);
///////////////////////////////////////////////////////

    printw("Bem vindo ao cliente.\n");
    getch();
    endwin();

    printf("Exiting...\n");
    exitNow();
    return 0;
}
/// NOMES E FIFOS
/*
    os fifos tem de ser para cada lado , cria se 1 para o gestor receber mensagens e os clientes criam os seus fifos para receberem mensagens, para verificar os nomes  dos users listara se todos os fifos do diretorio actual e verificar se algum tem o mesmo nome que o user, se tiver da se mensagem de erro se nao cria se um fifo com o nome do user e aceita se o seu nome como unico
*/
