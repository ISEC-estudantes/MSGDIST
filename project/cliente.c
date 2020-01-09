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
    terminar(info);
}

//le o newnot
void pipereadprob(){
    newnot(info, "[ERROR]O gestor nao conseguiu ler o fifo.\nA desligar em 10 segs.",0,0);
    sleep(10);
    terminar(info);
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
    int i, ff_cliente, ff_gestor, debug = 0, help = 0, error = 0, fck = 0, ff_lixo, bytes = 0;

    error = getoption(argc, argv, &fck, &error, &help, &debug);
    if (debug == 1)
    {
        printf("MAIN-OPTIONS"
               "fck = %d\n"
               "help = %d\n"
               "debug = %d\n",
               fck, help, debug);
    }

    info->debug = debug;
    info->filter = fck;

    if (2 > argc)
    {
        printf("digite o seu nome: ");
        fflush(stdout);
        scanf("%99[^\n]", info->nome);
    }
    else
    {
        strcpy(info->nome, argv[1]);
        for (i = 2; i < argc; ++i)
        {
            strcat(info->nome, " ");
            strcat(info->nome, argv[i]);
        }
    }
    if (info->filter == 0)
    {
        ///////////////////////////////////////////////////////
        ///////////////COMUNICACAO COM O GESTOR////////////////

        if (mkfifo(pidchar, 0666) != 0)
        {
            printf("Unable to create a fifo");
            exit(-1);
        }
        ff_cliente = open(pidchar, O_RDWR);
        ff_cliente = open(pidchar, O_RDONLY);
        ff_lixo = open(pidchar, O_WRONLY);
        if (info->debug == 1)
            fprintf(stdout, "fifo criado com sucesso\n");

        //criar o pipemsg//

        pipemsg envrcb ;

        envrcb.codigo = ADD_CLIENT;
        strcpy(envrcb.clientname, info->nome);
        envrcb.pid = info->pid;

        //escreve para o fifo servidor//
        ff_gestor = open("gestor-fifo", O_WRONLY);
        printf(" ff_gestor = %d\n", ff_gestor);
        if (ff_gestor > -1)
        {

            bytes = write(ff_gestor, (void *)&envrcb, sizeof(pipemsg));
            if (info->debug == 1)
                printf("sent %d bytes with the value %d, pid = %d\n", bytes, envrcb.codigo, envrcb.pid);
        }
        else
        {
            printf("O gestor nao esta em execucao.\n");
            unlink(pidchar);
            return 2;
        }

        //aguardar resposta

        read(ff_cliente, &envrcb, sizeof(pipemsg));
        if (info->debug == 1)
            printf("sent %d bytes with the value %d, pid = %d\n", bytes, envrcb.codigo, envrcb.pid);
        if (envrcb.codigo == 0)
        {
            printf("Connectado ao gestor, a iniciar ncurses...\n");
            strcpy(info->nome, envrcb.clientname);
        }
        else if (envrcb.codigo == INVALID_CLIENT_MAX)
        {
            printf("Maximo de clientes ligados ao gestor atingido, porfavor tente mais tarde.\n");
            unlink(pidchar);
            return 2;
        }
        else if (envrcb.codigo == INVALID_CLIENT_NAME)
        {
            printf("Existem 1000 pessoas com o mesmo nome do que tu, porfavor tenta algo mais original.\n");
            unlink(pidchar);
            return 2;
        }
        else
        {
            printf("Problemas no gestor, a desligar cliente.\n");
            int temp = i;
            char output[4];
            strcpy(output, ((char *)&temp));
            output[3] = '\0';
            unlink(pidchar);
            return 2;
        }
    }
    char lenonbre[50 + 15];
    sprintf(lenonbre, "O seu nome é %s.", info->nome);
    printf("%s\n", lenonbre);
    info->fifo_cliente = ff_cliente;
    info->fifo_gestor = ff_gestor;
    /* - -  - - - - - - -  - - - - --  --  --  --  - - - - - - - - -/
    /                    EDITOR DE TEXTO                            /
    /- - - - - -  - - - - - - - - - - - - - - - - - - - - --  -- - */
    //////////////// TRATAMENTO DE SINAIS ///////////////////
    signal(SIGINT, exitNow);
    signal(SIGPIPE, exitNow);
    signal(SIGUSR2, pipereadprob);
    ///////////////////////////////////////////////////////

    initscr();
    cbreak();
    //noecho();
    //buscar os maximos
    getmaxyx(stdscr, info->maxy, info->maxx); //y - linhas
    if (fck == 1)
        printw("maxy = %d, maxx = %d", info->maxy, info->maxx);
    refresh();

    if (info->maxy < 15 || info->maxx < 100)
    {
        printw("O cliente tem de ter no minimo 100 colunas e 15 linhas.\n");
        printw("Pressione qualquer tecla para continuar...");
        refresh();
        getchar();
        exitNow();
    }

    //criar as janelas
    info->mainwin = newwin(10, info->maxx, 0, 0);
    info->notification = newwin(info->maxy - 12, info->maxx, 12, 0);
    info->notificationborder = newwin(1, info->maxx, 11, 0);

    //fazer a border entre o main win e as notificacoes
    for (i = 0; i < info->maxx; ++i)
        wprintw(info->notificationborder, "-");
    wrefresh(info->notificationborder);

    wclear(info->notification);
    wclear(info->mainwin);
    wrefresh(info->notification);
    wrefresh(info->mainwin);
    wrefresh(info->mainwin);
    
    newnot(info, lenonbre, info->maxy - 12, info->maxx);
    
    //printa o nome
    // wprintw(info->notification, "O seu nome é %s.", info->nome);
    wrefresh(info->notification);
    keypad(info->mainwin, true);
    noecho();
    if (info->filter != 1)
        pthread_create(&info->threads, NULL, (void *)readingfifo, (void *)info);
    pthread_create(&info->threads, NULL, (void *)ui, (void *)info);
    pthread_join(info->threads, NULL);
    terminar(info);
    return 0;
}
/// NOMES E FIFOS
/*
    os fifos tem de ser para cada lado , cria se 1 para o gestor receber mensagens e os clientes criam os seus fifos para receberem mensagens, para verificar os nomes  dos users listara se todos os fifos do diretorio actual e verificar se algum tem o mesmo nome que o user, se tiver da se mensagem de erro se nao cria se um fifo com o nome do user e aceita se o seu nome como unico
*/
