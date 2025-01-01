#include <ncurses.h>

int main()
{
    int x, y;
    initscr();
    clear();

    getyx(stdscr, y, x);
    printw("x = %d\t y = %d", x, y);
    refresh();
    
    y = 5;
    x = 10;

    move(y, x);
    printw("over here!");
    refresh();

    getch();
    endwin();
    return 0;
}