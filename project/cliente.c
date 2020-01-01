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
#include "./headers/cfrt.h"
#include "./headers/editor.h"

global *info = NULL;

void exitNow()
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
    info = initinfo();
    char pidchar[10];
    info->pid = getpid();
    sprintf(pidchar, "%d", info->pid);
    int i, ff_cliente, ff_gestor, debug = 0,  help = 0, error = 0, fck = 0, ff_lixo, bytes = 0;


    error = getoption(argc, argv, &fck, &error, &help, &debug);
    if (debug == 1) {
        printf("MAIN-OPTIONS"
               "fck = %d\n"
               "help = %d\n"
               "debug = %d\n"
               , fck, help, debug
              );
    }

    info->debug = debug;
    info->filter = fck;

    if (2 > argc) {
        printf("digite o seu nome: ");
        fflush(stdout);
        scanf("%99[^\n]", info->nome);
    } else {
        strcpy(info->nome, argv[1]);
        for (i = 2; i < argc; ++i) {
            strcat(info->nome, " ");
            strcat(info->nome, argv[i]);
        }
    }
    if (fck == 0) {
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
        strcpy(envrcb.clientname, info->nome);
        envrcb.pid = info->pid;


        //escreve para o fifo servidor//
        ff_gestor = open("gestor-fifo", O_WRONLY);
        printf(" ff_gestor = %d\n", ff_gestor);
        if (ff_gestor > -1) {

            bytes = write(ff_gestor, (void *) &envrcb, sizeof(pipemsg));
            if (info->debug == 1)
                printf("sent %d bytes with the value %d, pid = %d\n", bytes, envrcb.codigo, envrcb.pid);

        } else {
            printf("O gestor nao esta em execucao.\n");
            unlink(pidchar);
            return 2;
        }


        //aguardar resposta

        read(ff_cliente, &envrcb, sizeof(pipemsg));
        if (info->debug == 1)
            printf("sent %d bytes with the value %d, pid = %d\n", bytes, envrcb.codigo, envrcb.pid);
        if (envrcb.codigo == 0) {
            printf("Connectado ao gestor, a iniciar ncurses...\n");
            strcpy(info->nome, envrcb.clientname);
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
            unlink(pidchar);
            return 2;
        }
    }
    printf("O seu nome é %s.", info->nome);
    
    info->fifo_cliente =  ff_cliente;
    info->fifo_gestor = ff_gestor;
    /* - -  - - - - - - -  - - - - --  --  --  --  - - - - - - - - -/
    /                    EDITOR DE TEXTO                            /
    /- - - - - -  - - - - - - - - - - - - - - - - - - - - --  -- - */
////////////////TRATAMENTO DE SINAIS///////////////////
    signal(SIGINT, exitNow);
    signal(SIGPIPE, exitNow);
///////////////////////////////////////////////////////

    initscr();
    cbreak();
    noecho();
    //buscar os maximos
    getmaxyx(stdscr, info->maxy, info->maxx);
    //criar as janelas
    info->mainwin = newwin(info->maxy - 5, info->maxx, 0, 0);
    info->notification = newwin(3, info->maxx, info->maxy - 3, 0);
    info->notificationborder = newwin(1, info->maxx, info->maxy - 4, 0);
    wprintw(info->notification, "O seu nome é %s.", info->nome );
    wrefresh(info->notification);
    //fazer a border entre o main win e as notificacoes
    for (i = 0; i < info->maxx; ++i)
        wprintw(info->notificationborder, "-");
    wrefresh(info->notificationborder);
    if (info->filter != 1)
        pthread_create(&info->threads, NULL, (void *)readingfifo, (void *)info);


    pthread_create(&info->threads, NULL, (void *)ui, (void *)info);



    pthread_join(info->threads, NULL);

    endwin();

    exitNow();
    return 0;
}
/// NOMES E FIFOS
/*
    os fifos tem de ser para cada lado , cria se 1 para o gestor receber mensagens e os clientes criam os seus fifos para receberem mensagens, para verificar os nomes  dos users listara se todos os fifos do diretorio actual e verificar se algum tem o mesmo nome que o user, se tiver da se mensagem de erro se nao cria se um fifo com o nome do user e aceita se o seu nome como unico
*/
