/*Trabalho de SO 2019 ISEC

Trabalho feito:
  João Gonçalves 21280302
  João Lopes     21270423

*/
//CLIENTE - MAIN
//fazer um gui

#include "utils.h"

void exitNow()
{
    char charrm[20], charpid[10];
    strcpy(charrm, "rm ");
    sprintf(charpid, "%d", getpid());
    strcat(charrm, charpid);
    system(charrm);
    if (access("gestor-fifo", F_OK))
    {
        int fd = open("gestor-fifo", O_WRONLY), erro = CLOSING_CLIENT;
        if (fd > 1)
        {
            write(fd, (void *)&erro, sizeof(int));
            erro = getpid();
            write(fd, (void *)&erro, sizeof(int));
            close(fd);
        }
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
    myinfo.ant = myinfo.prox = NULL;
    char rmcmd[20], pidchar[10];
    myinfo.pid = getpid();
    myinfo.prox = myinfo.ant = NULL;
    strcpy(rmcmd, "rm ");
    sprintf(pidchar, "%d", myinfo.pid);
    strcat(rmcmd, pidchar);
    int i, ff_cliente, ff_gestor;
    if (2 > argc)
    {
        printf("digite o seu nome: ");
        fflush(stdout);
        scanf("%99[^\n]", myinfo.nome);
    }
    else
    {
        strcpy(myinfo.nome, argv[1]);
        for (i = 2; i < argc; ++i)
        {
            strcat(myinfo.nome, " ");
            strcat(myinfo.nome, argv[i]);
        }
    }
    printf("este é o seu nome: %s\n", myinfo.nome);

    ///////////////////////////////////////////criacao do fifo///////////////////////////////////////////////////////

    signal(SIGINT, exitNow);
    if (mkfifo(pidchar, 0666) != 0)
    {
        printf("Unable to create a fifo");
        exit(-1);
    }
    fprintf(stdout, "fifo criado com sucesso\n");

    //escreve para o fifo servidor//
    if (access("gestor-fifo", W_OK) != -1)
    {
        i = ADD_CLIENT;
        ff_gestor = open("gestor-fifo", O_WRONLY);
        write(ff_gestor, (void *)&i, sizeof(int));
        write(ff_gestor, (void *)&myinfo, sizeof(cltusr));
        close(ff_gestor);
    }
    else
    {
        printf("O gestor nao esta em execucao.\n");
        system(rmcmd);
        return 2;
    }
    //aguardar resposta
    ff_cliente = open(pidchar, O_RDONLY);
    read(ff_cliente, &i, sizeof(int));
    if (i == 0)
    {
        printf("Connectado ao gestor, a iniciar ncurses...\n");
    }
    else if (i == -2)
    {
        printf("Maximo de clientes ligados ao gestor atingido, porfavor tente mais tarde.\n");
        system(rmcmd);
        return 2;
    }
    else if (i == -1)
    {
        printf("Existem 1000 pessoas com o mesmo nome do que tu, porfavor tenta algo mais original.\n");
        system(rmcmd);
        return 2;
    }
    else
    {
        printf("Problemas no gestor, a desligar cliente.\n");
        system(rmcmd);
        return 2;
    }

    /* - -  - - - - - - -  - - - - --  --  --  --  - - - - - - - -
        EDITOR DE TEXTO
     - - - - - -  - - - - - - - - - - - - - - - - - - - - --  -- - */
    initscr();
    printw("Bem vindo ao cliente.\n");
    getch();
    endwin();

    system(rmcmd);
    return 0;
}
/// NOMES E FIFOS
/*
    os fifos tem de ser para cada lado , cria se 1 para o gestor receber mensagens e os clientes criam os seus fifos para receberem mensagens, para verificar os nomes  dos users listara se todos os fifos do diretorio actual e verificar se algum tem o mesmo nome que o user, se tiver da se mensagem de erro se nao cria se um fifo com o nome do user e aceita se o seu nome como unico
    */