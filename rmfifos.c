
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <getopt.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "utils.h"

int main(int argc, char **argv)
{
    printf("a procura de fifos no diretorio actual...\n");

    int pr[2], cpid;

    if (pipe(pr) != 0)
        return -2;
    if ((cpid = fork()) == 0)
    {

        close(1);
        dup(pr[1]);
        close(pr[0]);
        close(pr[1]);

        //execl("/usr/bin/ls","ls", "-l", NULL);
        int error = system("ls -l | grep prw-r--r--");
        if (error != 0)
        {
            printf("erro\n");
        }
        exit(1);
    }
    if (cpid < 0)
    {
        fprintf(stderr, "nao consegui criar um fork.\n");
    }
    printf("a espera que acabe...\n");
    wait(&cpid);
    char rawinput[256], copia[256];
    int n = read(pr[0], rawinput, sizeof(rawinput)), i, f, g;
    rawinput[n - 1] = '\0';
    strcpy(copia, rawinput);
    if (strcmp(rawinput, "erro") == 0)
    {
        printf("nao existem fifos.\n");
        return 2;
    }

    int npipes = 0, counter;
    int maxusers = 10;
    char *pipes[maxusers];
    pipes[npipes] = strtok(rawinput, "\n");
    while ((pipes[++npipes] = strtok(NULL, "\n")) != NULL)
        ;

    for (i = 0; i < npipes; ++i)
    {
        for (f = 45, g = 0; f < strlen(pipes[i]); ++f, ++g)
        {
            pipes[i][g] = pipes[i][f];
        }
        pipes[i][g] = '\0';
        printf("pipe %d = %s\n", i + 1, pipes[i]);
    }
    printf("n de pipes = %d\n", npipes);
    strcpy(copia, "rm");
    
    if (argc > 1)
        if (argv[1][0] = '1')
        {
            for (i = 0; i < npipes; ++i)
            {
                strcat(copia, " ");
                strcat(copia, pipes[i]);
            }
            printf("sent: %s\n", copia);
            system(copia);
        }

    return 0;
    //46 caractes ate ao nome
}