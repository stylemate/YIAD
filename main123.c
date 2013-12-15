#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <strings.h>
#include "connector.h"
#define ESC 27 //27 is ASCII for Escape

bool IsPass(int y, int x);
void PlyrMv();
void Trail(int y, int x);
void DrawMap();
void DrawIntf();
void DrawMon(char mon, int quant);
void Intro(WINDOW* wNar);
WINDOW* wNar;
WINDOW* wStat;
WINDOW* wInfo;
WINDOW* wKey;

typedef struct mon
{
    char name[15];
    char shape;
    int CurHP, MaxHP;
    float atk, def;
    int exp;
}monster;

monster rat = {"Rat", 'r', 5, 5,1.5, 0, 1};


char *map[]={
    "############################################################",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "#                                                          #",
    "############################################################"
};

    //player's starting coordinates
    int y=1;
    int x=1;
    int c; //Key input

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
    DrawMon(rat.shape, 5);
    mvaddch(y,x,'@'); //draw character
    do
    {
        PlyrMv();
    }while(ESC != (c = getch()));

}


bool IsPass(int y, int x)
{
    if (map[y][x] == ' ')
        return true;
    else
        return false;
}

void PlyrMv()
{
        if (KEY_DOWN == c)
            if (IsPass(y+1,x))
                {
                    y++;
                    mvaddch(y,x,'@');  //draws the character
                    Trail(y-1,x);
                }
        if (KEY_UP == c)
            if (IsPass(y-1,x))
                {
                    y--;
                    mvaddch(y,x,'@');
                    Trail(y+1,x);
                }
        if (KEY_RIGHT == c)
            if (IsPass(y,x+1))
                {
                    x++;
                    mvaddch(y,x,'@');
                    Trail(y,x-1);
                }
        if (KEY_LEFT == c)
            if (IsPass(y,x-1))
                {
                    x--;
                    mvaddch(y,x,'@');
                    Trail(y,x+1);
                }
}

void Trail(int y, int x)
{
    if(map[y][x] == ' ')
        return mvaddch(y,x,' ');
    else
        return;
}

void DrawMap()
{
    int x,y;
    for (y=0;y<20;y++)
        for (x=0;x<60;x++)
            mvaddch(y,x,map[y][x]);
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
    refresh();
    wrefresh(wNar);
    wrefresh(wStat);
    wrefresh(wInfo);
    wrefresh(wKey);
}

void DrawMon(char mon,int quant)
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
            mvaddch(posY, posX, mon);
            refresh();
            counter++;
        }
    }
}
