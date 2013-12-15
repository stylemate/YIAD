#include <curses.h>
#include "connector.h"
char name[10];

void Intro(WINDOW* wNar)
{
    wborder(wNar, '|', '|', '-', '-', '*', '*', '*', '*');
    mvwaddstr(wNar,1,1,"What is your name? (limited to 10 letters)");
    refresh();
    wrefresh(wNar);
    echo();
    scanf("%s", name);
    noecho();
    mvwaddstr(wNar,1,1,"                                            ");
    mvwprintw(wNar,1,1,"Well then, %s, you will begin your adventure", name);
    refresh();
    wrefresh(wNar);
}
