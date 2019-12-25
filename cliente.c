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
    char charpid[10];

    sprintf(charpid, "%d", getpid());
    unlink(charpid);
    int fd = open("gestor-fifo", O_WRONLY), erro = CLOSING_CLIENT, bytes;
    if (fd > 1)
    {

        printf("exiting...\n");
        bytes = write(fd, (void *)&erro, sizeof(int));
#ifdef DEBUG
        printf("sent %d bytes with the value %d\n", bytes, erro);
#endif
        erro = getpid();
        bytes = write(fd, (void *)&erro, sizeof(int));
#ifdef DEBUG
        printf("sent %d bytes with the value %d\n", bytes, erro);
        printf("messages sent\n");
#endif
        close(fd);
    }
    else
    {
        printf("problemas a abrir para mandar a mensagem de fechar."
#ifdef DEBUG
               "[ERRO=%d]"
#endif
               "\n"
#ifdef DEBUG
               ,
               fd
#endif
        );
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
    char pidchar[10];
    myinfo.pid = getpid();
    myinfo.prox = myinfo.ant = NULL;
    sprintf(pidchar, "%d", myinfo.pid);
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
    i = ADD_CLIENT;
    ff_gestor = open("gestor-fifo", O_WRONLY);
    if (ff_gestor > -1)
    {
        write(ff_gestor, (void *)&i, sizeof(int));
        write(ff_gestor, (void *)&myinfo, sizeof(cltusr));
        close(ff_gestor);
    }
    else
    {
        printf("O gestor nao esta em execucao.\n");
        unlink(pidchar);
        return 2;
    }
    //aguardar resposta
    ff_cliente = open(pidchar, O_RDONLY);
    read(ff_cliente, &i, sizeof(int));

    if (i == 0)
    {
        printf("Connectado ao gestor, a iniciar ncurses...\n");
        read(ff_cliente, (void *)&myinfo.nome, sizeof(char));
    }
    else if (i == INVALID_CLIENT_MAX)
    {
        printf("Maximo de clientes ligados ao gestor atingido, porfavor tente mais tarde.\n");
        unlink(pidchar);
        return 2;
    }
    else if (i == INVALID_CLIENT_NAME)
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
        strcpy(output,((char*)&temp));
        output[3] = '\0';
        
        printf("MENSAGEM\n\tint = %d\n\t char = %s\n", i, output);
        unlink(pidchar);
        return 2;
    }

    /* - -  - - - - - - -  - - - - --  --  --  --  - - - - - - - -
        EDITOR DE TEXTO
     - - - - - -  - - - - - - - - - - - - - - - - - - - - --  -- - */

    initscr();
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
