/*Trabalho de SO 2019 ISEC

Trabalho feito:
  João Gonçalves 21280302
  João Lopes     21270423

*/
//CLIENTE - MAIN
//fazer um gui

#include "utils.h"

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
    strcpy(rmcmd, "rm ");
    sprintf(pidchar, "%d", myinfo.pid);
    strcat(rmcmd, pidchar);
    int i;
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
    int fifo_cliente;
    fifo_cliente = mkfifo(pidchar, 0666);

    if (fifo_cliente < 0)
    {
        printf("Unable to create a fifo");
        exit(-1);
    }
    fprintf(stdout, "fifo criado com sucesso\n");

    //escreve para o fifo servidor//
    if (access("gestorfifo", W_OK) != -1)
    {
        i=ADD_CLIENT;
        int ff_gestor = open("gestorfifo", O_WRONLY);
        write(ff_gestor, (void*)&i, sizeof(int));
        write(ff_gestor, (void*)&myinfo, sizeof(cltusr));
    }
    else
    {
        printf("O gestor nao esta em execucao.\n");
    }

    /* - -  - - - - - - -  - - - - --  --  --  --  - - - - - - - -
        EDITOR DE TEXTO
     - - - - - -  - - - - - - - - - - - - - - - - - - - - --  -- - */

    system(rmcmd);
    return 0;
}
    /// NOMES E FIFOS
    /*
    os fifos tem de ser para cada lado , cria se 1 para o gestor receber mensagens e os clientes criam os seus fifos para receberem mensagens, para verificar os nomes  dos users listara se todos os fifos do diretorio actual e verificar se algum tem o mesmo nome que o user, se tiver da se mensagem de erro se nao cria se um fifo com o nome do user e aceita se o seu nome como unico
    */