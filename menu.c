#include <curses.h>
#include "connector.h"

#define ENT 13
void DrawMenu()
{

    WINDOW* wRes;
    WINDOW* wLogo;
    WINDOW* wMenu;
    int x,y;
    int c, ch;
    char* logo[]=
    {
        "  YY    YY    IIIIIIII       AA       DDDDDD    ",
        "   YY  YY        II         AAAA      DD  DDD   ",
        "    YYYY         II        AA  AA     DD   DD   ",
        "     YY          II       AAAAAAAA    DD    DD  ",
        "     YY          II       AAAAAAAA    DD   DD   ",
        "     YY          II       AA    AA    DD  DDD   ",
        "     YY       IIIIIIII    AA    AA    DDDDDD    ",
    };
    //warning
    do
    {
        wRes = newwin(30,80,0,0);
        mvwaddstr(wRes,1,1,"Please resize terminal so that you can see the border");
        mvwaddstr(wRes,2,1,"Press any key to refresh,");
        mvwaddstr(wRes,3,1,"Press ENTER if it's resized");
        wborder(wRes, '|', '|', '-', '-', '*', '*', '*', '*');
        refresh();
        wrefresh(wRes);
    }while((c = getchar()) != ENT);
    clear();
    delwin(wRes);
    //logo
    wLogo = newwin(9,50,2,15);
    for (y=0;y<8;y++)
        for (x=0;x<48;x++)
            mvwaddch(wLogo,y+1,x+1,logo[y][x]);
    wborder(wLogo, '|', '|', '-', '-', '*', '*', '*', '*');

    //menu
    wMenu = newwin(10,40,16,20);
    wborder(wMenu, '|', '|', '-', '-', '*', '*', '*', '*');
    mvwaddstr(wMenu,3,14,"New  Game(n)");
    mvwaddstr(wMenu,6,14,"Load Game(l)");
    refresh();
    wrefresh(wLogo);
    wrefresh(wMenu);
    ch = getchar();
    switch(ch)
    {
        case 'n':
            break;
        case 'l':
            break;
    }
    //clean up
    delwin(wLogo);
    delwin(wMenu);
    clear();
}
