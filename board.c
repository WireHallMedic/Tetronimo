#include "board.h"

static board_struct_t board;
static int gameOver = FALSE;

#define BLOCK_CHAR 'X'
#define OPEN_CHAR ' '

int calcIndex(int x, int y)
{
	return x + (y * BOARD_WIDTH);
}

char getCell(int x, int y)
{
	return board.val[calcIndex(x, y)];
}

void setCell(int x, int y, char c)
{
	board.val[calcIndex(x, y)] = c;
}

void clearCompletedRows(void)
{
	int r = 0;
	for(r = 0; r < BOARD_HEIGHT; r++)
	{
		if(rowIsComplete(r))
			dropRow(r);
	}
}

int rowIsComplete(int r)
{
	int x = 0;
	for(x = 0; x < BOARD_WIDTH; x++)
	{
		if(getCell(x, r) == OPEN_CHAR)
			return FALSE;
	}
	return TRUE;
}

void dropRow(int r)
{
	int x = 0;
	int y = 0;
	for(y = r; y > 0; y--)
	for(x = 0; x < BOARD_WIDTH; x++)
	{
		setCell(x, y, getCell(x, y - 1));
	}
	for(x = 0; x < BOARD_WIDTH; x++)
		setCell(x, 0, OPEN_CHAR);
}

void clearBoard(void)
{
	int x = 0;
	int y = 0;
	for(x = 0; x < BOARD_WIDTH; x++)
	for(y = 0; y < BOARD_HEIGHT; y++)
		setCell(x, y, OPEN_CHAR);
}

void setBoardBoarders(void)
{
	int x = 0;
	int y = 0;
	for(x = 0; x <= BOARD_WIDTH; x++)
	{
		setScreenChar(x, -1, '-');
                setScreenChar(x, BOARD_HEIGHT, '-');
	}
        for(y = 0; y <= BOARD_HEIGHT; y++)
        {
                setScreenChar(-1, y, '|');
                setScreenChar(BOARD_WIDTH, y, '|');
        }
	setScreenChar(-1, -1, '+');
	setScreenChar(-1, BOARD_HEIGHT, '+');
	setScreenChar(BOARD_WIDTH, -1, '+');
	setScreenChar(BOARD_WIDTH, BOARD_HEIGHT, '+');

}

void setBoardContents(void)
{
	int x = 0;
	int y = 0;
	char ch = ' ';
	for(x = 0; x < BOARD_WIDTH; x++)
	for(y = 0; y < BOARD_HEIGHT; y++)
	{
		ch = getCell(x, y);
		setScreenChar(x, y, ch);
	}
}

void setPieceInBoard(piece_t *p, int x, int y)
{
	int w = 0;
	int h = 0;
	for(w = 0; w < 4; w++)
	for(h = 0; h < 4; h++)
	{
		if(occupies(p, w, h))
			setPlayChar(x + w, y + h, '@');
	}
}

int willCollide(piece_t *p, int x, int y, int orientation)
{
	int w = 0;
	int h = 0;
	for(w = 0; w < 4; w++)
	for(h = 0; h < 4; h++)
	{
		if(willOccupy(p, w, h, orientation))
		if(isOccupied(x + w, y + h))
			return TRUE;
	}
	return FALSE;
}

int isOccupied(int x, int y)
{
	/* not checking for y < 0 because above the board is non-colliding */
	if(x < 0  || x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
		return TRUE;
	return getCell(x, y) == BLOCK_CHAR;
}

void affixPiece(piece_t *p, int x, int y)
{
        int w = 0;
        int h = 0;
        for(w = 0; w < 4; w++)
        for(h = 0; h < 4; h++)
        {
                if(occupies(p, w, h))
		{
			if(y + h >= 0)
                        	setCell(x + w, y + h, BLOCK_CHAR);
        		else
				gameOver = TRUE;
		}
	}
}
/* wrapper because mvaddch is in the format of y, x */
void setScreenChar(int x, int y, char c)
{
	mvaddch(y + BOARD_DISP_INSET_Y, x + BOARD_DISP_INSET_X, c);
}

/* avoid writing outside of play area */
void setPlayChar(int x, int y, char c)
{
	if(y >= 0)
		setScreenChar(x, y, c);
}

int playerHasLost()
{
	return gameOver;
}
