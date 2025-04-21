#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>
#include <unistd.h>
#include "tetris.h"

char Table[ROWS][COLS] = {0};
int score = 0;
char GameOn = TRUE;
double timer = 500000;
Shape current = {NULL, 0, 0, 0};

int main()
{
    srand(time(0));
    score = 0;
    
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    while (1) {
        struct timeval before, after;
        gettimeofday(&before, NULL);

        GameOn = TRUE;
        ResetGame();
        PrintTable();

        while (GameOn) {
            int c = getch();
            if (c != ERR) {
                if (c == 'q' || c == 'Q') {
                    GameOn = FALSE;
                    break;
                }
                ManipulateCurrent(c);
            }
            gettimeofday(&after, NULL);
            if (((double)after.tv_sec * 1000000 + (double)after.tv_usec) -
                ((double)before.tv_sec * 1000000 + (double)before.tv_usec) > timer) {
                before = after;
                ManipulateCurrent('s');
            }
        }

        clear();
        mvprintw(ROWS / 2, (COLS - 10) / 2, "Game Over!");
        mvprintw(ROWS / 2 + 1, (COLS - 10) / 2, "Score: %d", score);
        mvprintw(ROWS / 2 + 2, (COLS - 10) / 2, "r: Restart, q: Quit");
        refresh();

        nodelay(stdscr, FALSE);
        int choice;
        while ((choice = getch()) != 'r' && choice != 'R' && choice != 'q' && choice != 'Q') {
        }
        nodelay(stdscr, TRUE);

        if (choice == 'q' || choice == 'Q') {
            break;
        }
    }

    if (current.array) {
        DeleteShape(current);
    }
    endwin();
    return 0;
}
