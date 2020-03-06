#include "piece.h"


/*
typedef struct piece_s
{
        char val[16 * 4];
        int orientation;
} piece_t;
*/

void setPiece(piece_t *p, int type, int orientation)
{
	switch(type)
	{
		case STRAIGHT :	strcpy(p->val,
				" X   X   X   X          XXXX     X   X   X   X          XXXX    ");
				break;
                case TEE : 	strcpy(p->val,
				"     X  XX   X       X  XXX          X   XX  X          XXX  X  ");
                                break;
                case BOX : 	strcpy(p->val,
				"         XX  XX          XX  XX          XX  XX          XX  XX ");
                                break;
                case ESS : 	strcpy(p->val,
				"     X   XX   X          XX XX       X   XX   X          XX XX  ");
                                break;
                case ZEE : 	strcpy(p->val,
				"     X  XX  X           XX   XX      X  XX  X           XX   XX ");
                                break;
                case ELL : 	strcpy(p->val,
				"    XX   X   X        X XXX          X   X   XX         XXX X   ");
                                break;
                case BACK_ELL : strcpy(p->val,
				"     XX  X   X          XXX   X      X   X  XX      X   XXX     ");
                                break;
	}
	p->orientation = 0;
}

void setRotation(piece_t *p, int orientation)
{
	p->orientation = orientation;
}

void rotate(piece_t *p)
{
	setRotation(p, getNextRotation(p->orientation));
}

int willOccupy(piece_t *p, int x, int y, int orientation)
{
        int index = orientation * 16;
        index += x;
        index += (y * 4);
        return p->val[index] == 'X';

}

int occupies(piece_t *p, int x, int y)
{
	return willOccupy(p, x, y, p->orientation);
}

int getNextRotation(int r)
{
	if(r == 3)
		return 0;
	return r + 1;
}

