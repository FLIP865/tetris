#include <stdlib.h>
#include <sys/time.h>
#include <ncurses.h>
#include "tetris.h"

const Shape ShapesArray[7] = {
    {(char *[]){(char []){0, 1, 1}, (char []){1, 1, 0}, (char []){0, 0, 0}}, 3},
    {(char *[]){(char []){1, 1, 0}, (char []){0, 1, 1}, (char []){0, 0, 0}}, 3},
    {(char *[]){(char []){0, 1, 0}, (char []){1, 1, 1}, (char []){0, 0, 0}}, 3},
    {(char *[]){(char []){0, 0, 1}, (char []){1, 1, 1}, (char []){0, 0, 0}}, 3},
    {(char *[]){(char []){1, 0, 0}, (char []){1, 1, 1}, (char []){0, 0, 0}}, 3},
    {(char *[]){(char []){1, 1}, (char []){1, 1}}, 2},
    {(char *[]){(char []){0, 0, 0, 0}, (char []){1, 1, 1, 1}, (char []){0, 0, 0, 0}, (char []){0, 0, 0, 0}}, 4}
};

Shape CoppyShape(Shape shape)
{
    Shape new_shape = shape;
    char **coppyshape = shape.array;
    new_shape.array = (char **)malloc(new_shape.width*sizeof(char *));
    if (!new_shape.array) {
        exit(1);
    }
    int i, j;
    for (i = 0; i < new_shape.width; i++) {
        new_shape.array[i] = (char *)malloc(new_shape.width * sizeof(char));
        if (!new_shape.array[i]) {
            exit(1);
        }
        for (j = 0; j < new_shape.width; j++) {
            new_shape.array[i][j] = coppyshape[i][j];
        }
    }
    return new_shape;
}

void DeleteShape(Shape shape)
{
    if (shape.array) {
        for (int i = 0; i < shape.width; i++) {
            free(shape.array[i]);
        }
        free(shape.array);
    }
}

int CheckPosition(Shape shape)
{
    char **array = shape.array;
    int i, j;
    for (i = 0; i < shape.width; i++) {
        for (j = 0; j < shape.width; j++) {
            if (shape.col + j < 0 || shape.col + j >= COLS || shape.row + i >= ROWS) {
                if (array[i][j]) {
                    return FALSE;
                }
            } else if (Table[shape.row + i][shape.col + j] && array[i][j]) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

void GetNewShape()
{
    Shape new_shape = CoppyShape(ShapesArray[rand() % 7]);
    new_shape.col = rand() % (COLS - new_shape.width + 1);
    new_shape.row = 0;
    
    if (current.array) {
        DeleteShape(current);
    }
    current = new_shape;
    if (!CheckPosition(current)) {
        GameOn = FALSE;
    }
}

void RotateShape(Shape shape)
{
    Shape temp = CoppyShape(shape);
    int i, j, k, width = shape.width;
    for (i = 0; i < width; i++) {
        for (j = 0, k = width - 1; j < width; j++, k--) {
            shape.array[i][j] = temp.array[k][i];
        }
    }
    DeleteShape(temp);
}

void WriteToTable()
{
    int i, j;
    for (i = 0; i < current.width; i++) {
        for (j = 0; j < current.width; j++) {
            if (current.array[i][j]) {
                Table[current.row + i][current.col + j] = current.array[i][j];
            }
        }
    }
}

void Halleluyah_Baby()
{
    int i, j, count = 0;
    for (i = ROWS - 1; i >= 0; i--) {
        int sum = 0;
        for (j = 0; j < COLS; j++) {
            sum += Table[i][j];
        }
        if (sum == COLS) {
            count++;
            for (int k = i; k > 0; k--) {
                for (j = 0; j < COLS; j++) {
                    Table[k][j] = Table[k - 1][j];
                }
            }
            for (j = 0; j < COLS; j++) {
                Table[0][j] = 0;
            }
            i++;
        }
    }
    score += 100 * count;
    if (count > 0) {
        timer = timer * 0.95;
        if (timer < 100000) {
            timer = 100000;
        }
    }
}

void PrintTable() 
{
    char buffer[ROWS][COLS] = {0};
    int i, j;
    
    if (current.array && GameOn) {
        for (i = 0; i < current.width; i++) {
            for (j = 0; j < current.width; j++) {
                if (current.array[i][j]) {
                    buffer[current.row + i][current.col + j] = current.array[i][j];
                }
            }
        }
    }

    clear();
    for (i = 0; i < ROWS + 2; i++) {
        for (j = 0; j < COLS + 2; j++) {
            if (i == 0 || i == ROWS + 1 || j == 0 || j == COLS + 1) {
                printw("# ");
            } else {
                printw("%c ", (Table[i - 1][j - 1] + buffer[i - 1][j - 1]) ? '#' : '.');
            }
        }
        printw("\n");
    }
    mvprintw(ROWS + 3, 0, "Score: %d", score);
    mvprintw(ROWS + 4, 0, "q: Quit, r: Restart");
    refresh();
}

void ResetGame()
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            Table[i][j] = 0;
        }
    }

    score = 0;
    timer = 500000;
    GameOn = TRUE;

    if (current.array) {
        DeleteShape(current);
        current.array = NULL;
    }
    GetNewShape();
}

void ManipulateCurrent(int action)
{
    if (action == 'r' || action == 'R') {
        ResetGame();
        PrintTable();
        return;
    }
    Shape temp = CoppyShape(current);
    switch (action) {
        case 's':
            temp.row++;
            if (CheckPosition(temp))
                current.row++;
            else {
                WriteToTable();
                Halleluyah_Baby();
                GetNewShape();
            }
            break;
        case 'd':
            temp.col++;
            if (CheckPosition(temp))
                current.col++;
            break;
        case 'a':
            temp.col--;
            if (CheckPosition(temp))
                current.col--;
            break;
        case 'w':
            RotateShape(temp);
            if (CheckPosition(temp))
                RotateShape(current);
            break;
    }
    DeleteShape(temp);
    if (GameOn) {
        PrintTable();
    }
}
