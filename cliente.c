/*Trabalho de SO 2019 ISEC

Trabalho feito:
  João Gonçalves 21280302
  João Lopes     21270423

*/
//CLIENTE - MAIN
//fazer um gui

#include "utils.h"

void welcome(){
    printf(
 " __  __ ___  ___ ___ ___ ___ _____            _ _         _       \n"
 "|  \\/  / __|/ __|   \\_ _/ __|_   _|  ___   __| (_)___ _ _| |_ ___ \n"
 "| |\\/| \\__ \\ (_ | |) | |\\__ \\ | |   |___| / _| | / -_) ' \\  _/ -_)\n"
 "|_|  |_|___/\\___|___/___|___/ |_|         \\__|_|_\\___|_||_\\__\\___|\n"
    );
}

void fhelp(){
    
}


int main(int argc, char **argv)
{
    welcome();
    /*  - - -  - - - - - - - - - - - - - - - -
        VERIFICADOR DO USER
    */
    //a minha implementaçao de um verificador de opcoes e valores nos argumento

    int i = 0, namefound = 0;
    for (; i < argc; ++i)
    { /*
        if (strcmp(argv[i], "-u") == 0)
        {*/
        if (i + 1 >= argc)
        {
            printf("Por favor digite um nome neste formato:\n"
            "./cliente o_seu_nome\n");
            return 1;
        }
        namefound = ++i;

        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        /* implementar verificacao do nome e mudança se preciso */
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        printf("este é o seu nome: %s\n", argv[namefound]);
    }

    if (namefound == 0)
    {
        printf("por favor insira -u como argumento com o seu nome à frente.\n");
        return 2;
    }

    /* - -  - - - - - - -  - - - - --  --  --  --  - - - - - - - -
        EDITOR DE TEXTO
     - - - - - -  - - - - - - - - - - - - - - - - - - - - --  -- - */
    
    /// NOMES E FIFOS
    /*
    os fifos tem de ser para cada lado , cria se 1 para o gestor receber mensagens e os clientes criam os seus fifos para receberem mensagens, para verificar os nomes  dos users listara se todos os fifos do diretorio actual e verificar se algum tem o mesmo nome que o user, se tiver da se mensagem de erro se nao cria se um fifo com o nome do user e aceita se o seu nome como unico
    */


    return 0;
}
