/*
 * Trabalho de SO 2019 ISEC
 *
 * Trabalho feito:
 *   João Gonçalves 21280302
 *   João Lopes     21270423
 *
*/
//CLIENTE - MSG EDITOR THREAD(S)

#include "./headers/editor.h"
#include "./headers/cfrt.h"

//tem de ter 100colunas*10linas characters(modificavel) para a main win
//1 para a separacao
//1 para a comand line
//1 para a separacao
//o resto para notificacoes para as notificacoes
//se nao tiver no minimo 100 colunas(maxx) e 15(maxy) linhas vai voltar e dar erro de nao ter espaco o soficiente

void ui(void *input)
{
    global *info = (global *)input;
    /*
    //printar os maximos
    wprintw(info->mainwin, "y=%d x=%d", info->maxy, info->maxx);
    wrefresh(info->mainwin);

    wclear(info->notification);
    wclear(info->mainwin);
    wrefresh(info->notification);
    wrefresh(info->mainwin);

    wprintw(info->mainwin, "\tQue deseja fazer?\n\n");
    wprintw(info->mainwin, "1 - Escrever nova mensagem.\n");
    wprintw(info->mainwin, "2 - Ver lista de topicos.\n"); //TODO - integrar um subscritor

    wprintw(info->mainwin, "3 - Ver lista de mesagens.\n"); //TODO - integrar um visualizador das mensagens
    wrefresh(info->mainwin);

    char option = getchar();
    wclear(info->mainwin);
    switch (option) {
    case '1':
        escrevermsg(info);
        break;
    case '2':
        wprintw(info->mainwin, "opcao 2");
        break;
    default:
        wprintw(info->mainwin, "lol not this");
        break;
    }
    wrefresh(info->mainwin);
    */
    //defenir as variaveis e as opcoes do menu
    int nopcoes = 4, i = 0, highlight = 0, choice = 0, choisen = 0, sair = 0;
    char opcoes[nopcoes][50];
    strcpy(opcoes[0], "1 - Escrever nova mensagem.");
    strcpy(opcoes[1], "2 - Ver lista de topicos.");
    strcpy(opcoes[2], "3 - Ver lista de mesagens.");
    strcpy(opcoes[nopcoes - 1], "4 - Sair");
    //menu
    while (!sair)
    {
        wclear(info->mainwin);
        wrefresh(info->mainwin);
        //mvwprintw(info->mainwin, 1, 3, "Que deseja fazer?");
        for (i = 0; i < nopcoes; ++i)
        {
            // if (i == highlight)
            //   wattron(info->mainwin, A_REVERSE);
            mvwprintw(info->mainwin, i + 2, 1, opcoes[i]);
            //wattroff(info->mainwin, A_REVERSE);
        }
        wrefresh(info->mainwin);
        choice = getch();

        switch (choice)
        {
        case 1:
            escrevermsg(info);
            break;
        case 2:
            wclear(info->mainwin);
            wprintw(info->mainwin, "A implementar(pressione uma tecla para continuar...)");
            wrefresh(info->mainwin);
            getchar();
            break;
        case 3:
            wclear(info->mainwin);
            wprintw(info->mainwin, "A implementar(pressione uma tecla para continuar...)");
            wrefresh(info->mainwin);
            getchar();
            break;
        case 4:
            wclear(info->mainwin);
            wprintw(info->mainwin, "Tem mesmo a certeza?");
            wprintw(info->mainwin, "S - sim     N - nao");
            wrefresh(info->mainwin);
            choice = tolower(getchar());
            if (choice == 's')
                sair = 1;
            break;
        }
    }
}

void vertopicos(global *info)
{
    wclear(info->mainwin);
    wprintw(info->mainwin, "A implementar(pressione uma tecla para continuar...)");
    wrefresh(info->mainwin);
    getchar();
}

void escrevermsg(global *info)
{
    //topicos
    //pthread_mutex_lock(&info->lock_tpc);
    wclear(info->mainwin);
    wrefresh(info->mainwin);
    info->maxtopics = 10;
    tpc *aux = info->listtopicos;

    int ntopicos = 0, i, sair = 0, choice = 0;
    char topics[10][50];

    if (info->debug == 1)
    {
        for (i = 0; i < info->nclientes; ++i)
            strcpy(topics[i], "boas");
        info->ntopicos = 10;
    }

    while (aux)
    {
        --ntopicos;
        sprintf(topics[ntopicos - 1], "%d - %s", ntopicos, aux->nome);
    }

    int highlight = 0;
    if (info->ntopicos > 0)
    {
        while (!sair)
        {
            for (i = 0; i < 10; ++i)
            {
                if (i == highlight)
                    wattron(info->mainwin, A_REVERSE);
                mvwprintw(info->mainwin, i + 1, 1, topics[i]);
                wattroff(info->mainwin, A_REVERSE);
            }
            choice = getch();
            switch (choice)
            {
            case ((int)'\n'):
            case 13:
            case KEY_ENTER:
                sair = 1;
                break;
            case KEY_UP:
                --highlight;
                if (highlight == -1)
                    highlight = 0;
                break;
            case KEY_DOWN:
                ++highlight;
                if (highlight == info->maxtopics)
                    highlight = info->maxtopics - 1;
                break;
            default:
                break;
            }
            if (choice == 1)
                break;
        }
        wclear(info->mainwin);
        wprintw(info->mainwin, "your choice was: %s", topics[highlight]);
        wrefresh(info->mainwin);
        getchar();
    }
    else
    {
        wprintw(info->mainwin, "nao existem topicos quer criar um novo?");
        char simnao[2][5];
        strcpy(simnao[0], "sim");
        strcpy(simnao[1], "nao");
        while (!sair)
        {

            for (i = 0; i < 2; ++i)
            {
                if (i == highlight)
                    wattron(info->mainwin, A_REVERSE);
                mvwprintw(info->mainwin, i + 1, 1, simnao[i]);
                wattroff(info->mainwin, A_REVERSE);
            }
            choice = wgetch(info->mainwin);

            switch (choice)
            {
            case ((int)'\n'):
            case 13:
            case KEY_ENTER:
                sair = 1;
                break;
            case KEY_UP:
                --highlight;
                if (highlight == -1)
                    highlight = 0;
                break;
            case KEY_DOWN:
                ++highlight;
                if (highlight == 2)
                    highlight = 1;
                break;
            default:
                break;
            }
            if (sair == 1)
                break;
        }
        if (highlight == 0)
        {
            addtpcui(info);
        }
        getchar();
    }
}

void addtpcui(void *input)
{
    global *info = (global *)input;
    echo();
    char topicname[50];
    wclear(info->mainwin);
    wprintw(info->mainwin, "Name of topic:\n > ");
    wgetstr(info->mainwin, topicname);

    noecho();
}

void newnot(global *info, char *msg, int nlinhas, int ncolunas)
{
    static pthread_mutex_t nots;
    static int mutexset = 0, isset = 0;
    if (mutexset == 0)
    {
        pthread_mutex_init(&nots, NULL);
        mutexset = 1;
    }
    static int linhas, colunas;
    static char **notificacoes = NULL;
    if (nlinhas == -1)
    {
        for (int i = 0; i < linhas; ++i)
            free(notificacoes[i]);
        free(notificacoes);
        return;
    }
    if (nlinhas != 0)
    {
        linhas = nlinhas;
        colunas = ncolunas;
        notificacoes = malloc(sizeof(char *) * linhas);
        if (!notificacoes)
        {
            printf("Sem mem.");
            exitNow();
        }
        for (int i = 0; i < linhas; ++i)
        {
            notificacoes[i] = malloc(sizeof(char) * colunas);
            if (!notificacoes[i])
            {
                printf("Sem mem.");
                exitNow();
            }
            notificacoes[i][0] = '\0';
        }
        isset = 1;
    }
    //6
    for (int i = nlinhas - 2; i >= 0; --i)
    {
        strcpy(notificacoes[i + 1], notificacoes[i]);
    }
    strcpy(notificacoes[0], msg);
    wclear(info->notification);
    for (int i = 0; i < linhas; ++i)
    {
        wprintw(info->notification, notificacoes[i]);
    }
}
