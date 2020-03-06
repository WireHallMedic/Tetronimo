#ifndef BOARD_DEF
#define BOARD_DEF

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define BOARD_DISP_INSET_X 3
#define BOARD_DISP_INSET_Y 3

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <curses.h>
#include "piece.h"

typedef struct board_struct_s
{
	char val[BOARD_HEIGHT * BOARD_WIDTH];
} board_struct_t;

char getCell(int x, int y);
int calcIndex(int x, int y);
void clearRow(int y);
void clearBoard(void);
void setBoardBoarders(void);
void setBoardContents(void);
void setScreenChar(int x, int y, char ch);
void setPieceInBoard(piece_t *p, int x, int y);
int willCollide(piece_t *p, int x, int y, int orientation);
int isOccupied(int x, int y);
void affixPiece(piece_t *p, int x, int y);
void setPlayChar(int x, int y, char c);
void clearCompletedRows(void);
int rowIsComplete(int r);
void dropRow(int r);
int playerHasLost(void);

#endif
