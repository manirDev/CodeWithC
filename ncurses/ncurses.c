#include <ncurses.h>


int main()
{
    char letter;
    initscr();
    printw("Press any key");
    refresh();

    letter = getch();
    clear();

    printw("You psuhed: '%c'", letter);
    refresh();

    getch();
    endwin();
    return 0;
}