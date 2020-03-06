#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <curses.h>
#include "board.h"
#include "piece.h"

void sigalrmHandler(int sig);
void sigintHandler(int sig);
void updateScreen(void);
void setTimer(void);
void dirPressed(int dir);
void nextPiece(void);
void applyGravity(void);
void drawNextPiece(void);
void exitProg(void);
