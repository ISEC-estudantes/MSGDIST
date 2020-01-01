
#include "./headers/utils.h"
#include "./headers/editor.h"


global *info = NULL;

int main()
{
    int i;
    info = initinfo();
    info->debug = 1;
    initscr();
    cbreak();
    noecho();
    //buscar os maximos
    getmaxyx(stdscr, info->maxy, info->maxx);
    //criar as janelas
    info->mainwin = newwin(info->maxy - 5, info->maxx, 0, 0);
    info->notification = newwin(3, info->maxx, info->maxy - 3, 0);
    info->notificationborder = newwin(1, info->maxx, info->maxy - 4, 0);

    //fazer a border entre o main win e as notificacoes
    for (i = 0; i < info->maxx; ++i)
        wprintw(info->notificationborder, "-");
    wrefresh(info->notificationborder);


    //printar os maximos
    wprintw(info->mainwin, "y=%d x=%d", info->maxy, info->maxx);
    wrefresh(info->mainwin);

    //printar a notificacao
    wprintw(info->notification, "this is a notification");
    wrefresh(info->notification);
    //pausar
    getchar();
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
    getchar();
    endwin();
    return 0;
}

