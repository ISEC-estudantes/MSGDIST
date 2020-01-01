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

//tem de ter 50colunas*20linas characters(modificavel)
void ui(void *input)
{
    global *info = (global *)input;

    /*
    //printar os maximos
    wprintw(info->mainwin, "y=%d x=%d", info->maxy, info->maxx);
    wrefresh(info->mainwin);
    */
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

    if (info->debug == 1) {
        for (i = 0; i < info->nclientes; ++i)
            strcpy(topics[i], "boas");
        info->ntopicos = 10;
    }

    while (aux) {
        --ntopicos;
        sprintf(topics[ntopicos - 1], "%d - %s", ntopicos, aux->nome);
    }

    keypad(info->mainwin, true);

    int  highlight = 0;
    if (info->ntopicos > 0) {
        while (!sair) {
            for (i = 0; i < 10; ++i) {
                if (i == highlight)
                    wattron(info->mainwin, A_REVERSE);
                mvwprintw(info->mainwin, i + 1, 1, topics[i]);
                wattroff(info->mainwin, A_REVERSE);
            }
            choice = getch();

            switch (choice) {
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

    } else {
        wprintw(info->mainwin, "nao existem topicos quer criar um novo?");
        char simnao[2][5];
        strcpy(simnao[0], "sim");
        strcpy(simnao[1], "nao");
        while (!sair) {

            for (i = 0; i < 2; ++i) {
                if (i == highlight)
                    wattron(info->mainwin, A_REVERSE);
                mvwprintw(info->mainwin, i + 1, 1, simnao[i]);
                wattroff(info->mainwin, A_REVERSE);
            }
            choice = wgetch(info->mainwin);

            switch (choice) {
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
        if(highlight == 0){
            addtpcui(info);
        }
        getchar();
    }
}

void addtpcui(void * input){
    global * info = (global *)input;
    echo();
    char topicname[50];
    wclear(info->mainwin);
    wprintw(info->mainwin, "Name of topic:\n > ");
    wgetstr(info->mainwin, topicname);
    
    noecho();
}
