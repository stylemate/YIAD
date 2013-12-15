#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <strings.h>
#include "connector.h"

void FillInfo(WINDOW* wInfo, int stage)
{
    switch (stage)
    {
        case 1:
            mvwaddstr(wInfo,1,3,"@ = Player");
            mvwaddstr(wInfo,2,3,"# = Wall");
            mvwaddstr(wInfo,3,3,"o = Next Floor");
            mvwaddstr(wInfo,4,3,"! = Potion");
            mvwaddstr(wInfo,5,3,"r = Rat");
            mvwaddstr(wInfo,6,3,"p = Pencil");
            break;
    }
    refresh();
    wrefresh(wInfo);
}
