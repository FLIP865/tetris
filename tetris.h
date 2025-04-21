#ifndef TETRIS_H
#define TETRIS_H

#define ROWS 20
#define COLS 11
#define TRUE 1
#define FALSE 0

typedef struct {
    char **array;
    int width, row, col;
} Shape;
extern Shape current;
extern Shape current;
extern char Table[ROWS][COLS];
extern int score;
extern char GameOn;
extern double timer;
extern const Shape ShapesArray[7];

Shape CoppyShape(Shape shape);
void DeleteShape(Shape shape);
int CheckPosition(Shape shape);
void GetNewShape();
void RotateShape(Shape shape);
void WriteToTable();
void Halleluyah_Baby();
void PrintTable();
void ManipulateCurrent(int action);

#endif
