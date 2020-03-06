#ifndef PIECE_DEF
#define PIECE_DEF

#include <string.h>

#define STRAIGHT 0
#define TEE 1
#define BOX 2
#define ESS 3
#define ZEE 4
#define ELL 5
#define BACK_ELL 6

typedef struct piece_s
{
	char val[65]; /* room for the null character, though we don't use it */
	int orientation;
} piece_t;

void setPiece(piece_t *p, int type, int orientation);
void setRotation(piece_t *p, int orientation);
void rotate(piece_t *p);
int occupies(piece_t *p, int x, int y);
int willOccupy(piece_t *p, int x, int y, int orientation);
int getNextRotation(int r);

#endif
