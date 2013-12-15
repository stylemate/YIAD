#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <strings.h>
#include "connector.h"
#define ESC 27 //27 is ASCII for Escape
#define ATK 97 // a
#define FLOOR 0
#define WALL 1
#define RAT 2

bool IsPass(int y, int x);
bool MonPass(WINDOW* wMap, int y, int x);
void PlyrMv();
void Atk();
void Trail(int y, int x);
void DrawMap();
void DrawIntf();
void DrawMon(char mon, int quant);
void Intro(WINDOW* wNar);
void ClrNr();
WINDOW* wNar;
WINDOW* wStat;
WINDOW* wInfo;
WINDOW* wKey;
WINDOW* wMap;
typedef struct
{
    int CurHP, MaxHP;
    float atk, def;
    int CurExp, MaxExp;
}plyr;

//initialization
plyr player = {15, 15, 2, 0, 0, 5};

typedef struct
{
    char name[15];
    char shape;
    int CurHP, MaxHP;
    float atk, def;
    int exp;
}monster;

monster rat = {"Rat", 'r', 5, 5,1.5, 0, 1};


int map[20][60]=
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
    //player's starting coordinates
    int y = 1;
    int x = 1;
    int c, a, e; //Key input

void main() {

    //initialize curses
    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    curs_set(0);

    //draw logo
    DrawMenu();
    DrawIntf();
    Intro(wNar);


    DrawMap();  //draw map
    FillInfo(wInfo, 1);
    mvwaddch(wMap, y,x,'@'); //draw character
    refresh();
    wrefresh(wMap);
    do
    {
        do
        {
            PlyrMv();
        }while(ATK != (c = getch()));
        Atk();
        DrawMap();
    }while(ESC != (c =getch()));

}


bool IsPass(int y, int x)
{
    if (map[y][x] == WALL || map[y][x] == RAT)
        return false;
    else
        return true;
}

void PlyrMv()
{
        if (KEY_DOWN == c)
            if (IsPass(y+1,x))
                {
                    ClrNr();
                    y++;
                    mvwaddch(wMap,y,x,'@');  //draws the character
                    Trail(y-1,x);
                }
        if (KEY_UP == c)
            if (IsPass(y-1,x))
                {
                    ClrNr();
                    y--;
                    mvaddch(y,x,'@');
                    Trail(y+1,x);
                }
        if (KEY_RIGHT == c)
            if (IsPass(y,x+1))
                {
                    ClrNr();
                    x++;
                    mvaddch(y,x,'@');
                    Trail(y,x-1);
                }
        if (KEY_LEFT == c)
            if (IsPass(y,x-1))
                {
                    ClrNr();
                    x--;
                    mvaddch(y,x,'@');
                    Trail(y,x+1);
                }
        refresh();
        wrefresh(wMap);
        wrefresh(wNar);
}

void Atk()
{
    int deltax = 0, deltay = 0;
    switch(c)
    {
        case 97:
            mvwaddstr(wNar,1,1,"Which direction do you want to attack?        ");
            mvwprintw(wNar,2,1,"                                        ");
            refresh();
            wrefresh(wNar);
            a = getch();
            if (KEY_DOWN == a)
                {
                    mvwprintw(wNar,1,1,"You attacked downward and dealt %.1f damage", player.atk);
                    mvwprintw(wNar,2,1,"Press Enter");
                    deltay = 1;
                }
            if (KEY_UP == a)
                {
                    mvwprintw(wNar,1,1,"You attacked upward and dealt %.1f damage", player.atk);
                    mvwprintw(wNar,2,1,"Press Enter");
                    deltay = -1;
                }
            if (KEY_RIGHT == a)
                {
                    mvwprintw(wNar,1,1,"You attacked downward and dealt %.1f damage", player.atk);
                    mvwprintw(wNar,2,1,"Press Enter");
                    deltax = 1;
                }
            if (KEY_LEFT == a)
                {
                    mvwprintw(wNar,1,1,"You attacked downward and dealt %.1f damage", player.atk);
                    mvwprintw(wNar,2,1,"Press Enter");
                    deltax = -1;
                }
            refresh();
            wrefresh(wNar);
            if (map[y + deltay][x + deltax] == RAT)
            {
                rat.CurHP -= player.atk;
                if (rat.CurHP <= 0)
                {
                    map[y + deltay][x + deltax] = FLOOR;
                    mvwaddch(wMap, y + deltay, x + deltax, ' ');
                }
                refresh();
                wrefresh(wMap);
                wrefresh(wStat);
            }
            break;
    }
}


void Trail(int y, int x)
{
        return mvaddch(y,x,' ');
}

void DrawMap()
{
    int xx,yy;
    for (yy=0;yy<20;yy++)
        for (xx=0;xx<60;xx++)
        {
            switch(map[yy][xx])
            {
                case WALL:
                    mvwaddch(wMap, yy, xx, '#');
                    break;
                case RAT:
                    mvwaddch(wMap, yy, xx, 'r');
                    break;
            }
            refresh();
            wrefresh(wMap);
        }
}

void DrawIntf()
{
    wNar = newwin(10, 60, 20, 0);
    wborder(wNar, '|', '|', '-', '-', '*', '*', '*', '*');
    wStat = newwin(7, 20, 0, 60);
    wborder(wStat, '|', '|', '-', '-', '*', '*', '*', '*');
    wInfo = newwin(13, 20, 7, 60);
    wborder(wInfo, '|', '|', '-', '-', '*', '*', '*', '*');
    wKey = newwin(10, 20, 20, 60);
    wborder(wKey, '|', '|', '-', '-', '*', '*', '*', '*');
    wMap = newwin(20, 60, 0, 0);
    refresh();
    wrefresh(wNar);
    wrefresh(wStat);
    wrefresh(wInfo);
    wrefresh(wKey);
    wrefresh(wMap);
}

void ClrNr()
{
    mvwprintw(wNar,1,1, "                                                          ");
    mvwprintw(wNar,2,1, "                                                          ");
    mvwprintw(wNar,3,1, "                                                          ");
    mvwprintw(wNar,4,1, "                                                          ");
    mvwprintw(wNar,5,1, "                                                          ");
    mvwprintw(wNar,6,1, "                                                          ");
    mvwprintw(wNar,7,1, "                                                          ");
    mvwprintw(wNar,8,1, "                                                          ");
}
/*void DrawMon(char mon,int quant)
{
    int posY, posX;
    int counter = 0;
    srand(time(NULL));
    while (counter < quant)
    {
        posY = (rand()%20);
        posX = (rand()%60);
        if (IsPass(posY, posX))
        {
            mvwaddch(wMap, posY, posX, mon);
            refresh();
            wrefresh(wMap);
            counter++;
        }
    }
}
*/
