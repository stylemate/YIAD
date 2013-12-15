#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <strings.h>
#include "connector.h"
#define ESC 27 //27 is ASCII for Escape
#define ATK 97 // a
#define INT 105 //i
#define FLOOR 0
#define WALL 1
#define RAT 2
#define POTION 3
#define PENCIL 4
#define NEXT 99
#define RATNUM 11
#define PENNUM 5

typedef struct Plyr
{
    float CurHP, MaxHP;
    float atk, def;
    int CurExp, MaxExp;
}plyr;

//initialization
plyr player = {15, 15, 2, 0, 0, 3};
int lvl = 1; //player level

typedef struct
{
    char name[15];
    char shape;
    float CurHP, MaxHP;
    float atk, def;
    int exp;
}monster;

bool IsPass(int y, int x);
bool MonPass(WINDOW* wMap, int y, int x);
void PlyrMv();
void Atk();
char Trail(int y, int x);
void DrawMap();
void DrawIntf();
void DrawMon(char mon, int quant);
void Intro(WINDOW* wNar);
void ClrNr();
void ClrStat();
void FillStat(plyr player);
void StatCheck();
void LvlUp();
monster SetMon(char name[15], char shape, float CurHP, float MaxHP, float atk, float def, int exp);
void MonLoop();
void End();
WINDOW* wNar;
WINDOW* wStat;
WINDOW* wInfo;
WINDOW* wKey;
WINDOW* wMap;

int i = 0, j = 0; //counter for monster
int aa=0,bb=0,cc=0,dd=0,ee=0,ff=0,gg=0,hh=0;


monster rat[RATNUM];
monster pencil[PENNUM];




int map[20][60]=
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,3,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,4,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,2,0,0,0,0,1},
    {1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,2,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,2,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,4,0,0,0,0,1},
    {1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,2,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,0,4,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1},
    {1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,2,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,4,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,2,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,2,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,4,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,99,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
    //player's starting coordinates
    int y = 1;
    int x = 1;
    int c, a, e; //Key input

int main() {

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
    FillStat(player);
    mvwaddch(wMap, y,x,'@'); //draw character
    refresh();
    wrefresh(wMap);
    MonLoop();

    do
    {
        do
        {
            PlyrMv();
            StatCheck();
        }while((ATK != (c = getch())));
        StatCheck();
        Atk();
        DrawMap();
        LvlUp();
        StatCheck();
        FillStat(player);
    }while(ESC != (c =getch()));
    //clean up
    delwin(wNar);
    delwin(wStat);
    delwin(wInfo);
    delwin(wKey);
    delwin(wMap);
    return 0;
}


bool IsPass(int y, int x)
{
    if (map[y][x] == WALL || map[y][x] == RAT || map[y][x] == PENCIL)
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
        if (map[y][x] == POTION)
        {
            map[y][x] = FLOOR;
            mvwaddch(wMap, y, x, '@');
            ClrNr();
            mvwprintw(wNar, 1, 1, "You drank the potion and healed you for 5 HP");
            player.CurHP += 5;
            FillStat(player);
            DrawMap();
        }
        if (map[y][x] == NEXT)
        {
            End();
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
            mvwaddstr(wNar,2,1,"                                                  ");
            mvwaddstr(wNar,3,1,"                                                  ");
            mvwaddstr(wNar,2,1,"Which direction do you want to attack?");
            refresh();
            wrefresh(wNar);
            a = getch();
            if (KEY_DOWN == a)
                {
                    mvwprintw(wNar,2,1,"You attacked downward and dealt %.1f damage", player.atk);
                    mvwprintw(wNar,3,1,"Press Enter");
                    deltay = 1;
                }
            if (KEY_UP == a)
                {
                    mvwprintw(wNar,2,1,"You attacked upward and dealt %.1f damage", player.atk);
                    mvwprintw(wNar,3,1,"Press Enter");
                    deltay = -1;
                }
            if (KEY_RIGHT == a)
                {
                    mvwprintw(wNar,2,1,"You attacked downward and dealt %.1f damage", player.atk);
                    mvwprintw(wNar,3,1,"Press Enter");
                    deltax = 1;
                }
            if (KEY_LEFT == a)
                {
                    mvwprintw(wNar,2,1,"You attacked downward and dealt %.1f damage", player.atk);
                    mvwprintw(wNar,3,1,"Press Enter");
                    deltax = -1;
                    }
            refresh();
            wrefresh(wNar);
            if (map[y + deltay][x + deltax] == RAT)
            {
                rat[j].CurHP -= player.atk;
                mvwprintw(wNar,1,1,"Rat: %.1f/%.1f ATK:%.1f DEF:%.f EXP:%d", rat[j].CurHP, rat[j].MaxHP, rat[j].atk, rat[j].def, rat[j].exp);
                if (rat[j].CurHP <= 0)
                {
                    map[y + deltay][x + deltax] = FLOOR;
                    mvwaddch(wMap, y + deltay, x + deltax, ' ');
                    player.CurExp += rat[j].exp;
                    FillStat(player);
                    j++;
                }
                if(map[y + deltay][x + deltax] == RAT)
                {
                    player.CurHP -= rat[j].atk;
                    mvwprintw(wNar,2,1,"Rat attacked you and dealt %.1f damage     ", rat[j].atk);
                }

                refresh();
                wrefresh(wMap);
                wrefresh(wStat);
            }
            if (map[y + deltay][x + deltax] == PENCIL)
            {
                pencil[aa].CurHP -= player.atk;
                mvwprintw(wNar,1,1,"Pencil: %.1f/%.1f ATK:%.1f DEF:%.f EXP:%d", pencil[aa].CurHP, pencil[aa].MaxHP, pencil[aa].atk, pencil[aa].def, pencil[aa].exp);
                if (pencil[aa].CurHP <= 0)
                {
                    map[y + deltay][x + deltax] = FLOOR;
                    mvwaddch(wMap, y + deltay, x + deltax, ' ');
                    player.CurExp += pencil[aa].exp;
                    FillStat(player);
                    aa++;
                }
                if(map[y + deltay][x + deltax] == PENCIL)
                {
                    player.CurHP -= pencil[aa].atk;
                    mvwprintw(wNar,2,1,"Pencil attacked you and dealt %.1f damage     ", pencil[aa].atk);
                }
                refresh();
                wrefresh(wMap);
                wrefresh(wStat);
            }
            break;
    }
}


char Trail(int y, int x)
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
                case POTION:
                    mvwaddch(wMap, yy, xx, '!');
                    break;
                case PENCIL:
                    mvwaddch(wMap, yy, xx, 'p');
                    break;
                case NEXT:
                    attron(COLOR_PAIR(1));
                    mvwaddch(wMap, yy, xx, 'o');
                    attroff(COLOR_PAIR(1));
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
void ClrStat()
{
    mvwprintw(wStat,1,1, "                  ");
    mvwprintw(wStat,2,1, "                  ");
    mvwprintw(wStat,3,1, "                  ");
    mvwprintw(wStat,4,1, "                  ");
    mvwprintw(wStat,5,1, "                  ");
}

void FillStat(plyr player)
{
    mvwprintw(wStat, 1, 5, "LVL: %d", lvl);
    mvwprintw(wStat, 2, 5, "HP: %.1f/%.1f", player.CurHP, player.MaxHP);
    mvwprintw(wStat, 3, 5, "ATK: %.1f", player.atk);
    mvwprintw(wStat, 4, 5, "DEF: %.1f", player.def);
    mvwprintw(wStat, 5, 5, "EXP: %d/%d", player.CurExp, player.MaxExp);
    refresh();
    wrefresh(wStat);
}

void StatCheck()
{
    if(player.CurHP > player.MaxHP)
        player.CurHP = player.MaxHP;
    FillStat(player);
}

monster SetMon(char name[15], char shape, float CurHP, float MaxHP, float atk, float def, int exp)
{
    monster temp;
    temp.CurHP = CurHP;
    temp.MaxHP = MaxHP;
    temp.atk = atk;
    temp.def = def;
    temp.exp = exp;
    return (temp);
}

void MonLoop()
{
    for(i = 0; i < RATNUM; i++)
        rat[i] = SetMon("Rat", 'r', 5, 5, 2, 0, 3);
    for(i = 0; i < PENNUM; i++)
        pencil[i] = SetMon("Pencil", 'p', 4, 4, 3, 0, 5);
}

void LvlUp()
{
    if(player.CurExp >= player.MaxExp)
    {
        lvl++;
        player.CurExp = (player.CurExp - player.MaxExp);
        player.MaxExp = 3*lvl*lvl;
        player.MaxHP += 2.5;
        player.CurHP = player.MaxHP;
        player.atk += 1.75;
        player.def += 1;
        ClrStat();
        FillStat(player);
        ClrNr();
        mvwaddstr(wNar, 1, 1, "Level Up!");
        refresh();
        wrefresh(wNar);
    }
}
void End()
{
    clear();
    mvprintw(0,0,"Hope you liked my work. Press any key to exit");
    bb = getch();
    exit(0);
}
