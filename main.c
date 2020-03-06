#include "main.h"
#include "board.h"
#include "piece.h"

#define TIMESTEP 250000 /* quarter of a second per tick */
#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3
#define INSTRUCTION_INSET_X 20
#define INSTRUCTION_INSET_Y 4
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define BOARD_DISP_INSET_X 3
#define BOARD_DISP_INSET_Y 3

piece_t curPiece;
int nextPieceInt;
int curX;
int curY;
static WINDOW *boardWindow = NULL;
static WINDOW *nextPieceWindow = NULL;

int main(int argc, char *argv[])
{
	void *sig_return_val = NULL;
	int keyPressed; /* int not char to be able to use arrows*/

	/* set signal handler for timer */
	sig_return_val = signal(SIGALRM, sigalrmHandler);
	if(sig_return_val == SIG_ERR)
        {
        	printf("Failed to register signal handler.\n");
		return 0;
	}

        /* set signal handler for sigint */
        sig_return_val = signal(SIGINT, sigintHandler);
        if(sig_return_val == SIG_ERR)
        {
                printf("Failed to register signal handler.\n");
                return 0;
        }

	/* initialize curses */
	initscr();		/* initialize curses */
	cbreak();		/* don't wait for \n to read buffer */
	noecho();		/* don't echo key input to screen */
	nodelay(stdscr, TRUE);	/* getch() is non-blocking, returns NULL if empty */
	keypad(stdscr, TRUE);	/* enable getch() to return arrow keys */
	curs_set(0);		/* set cursor to invisible */
        clear();                /* clear screen */
	boardWindow = newwin(BOARD_HEIGHT, BOARD_WIDTH, BOARD_DISP_INSET_Y, BOARD_DISP_INSET_X);
	nextPieceWindow = newwin(4, 4, INSTRUCTION_INSET_Y + 1, INSTRUCTION_INSET_X + 4);

	/* directions */
	mvaddstr(INSTRUCTION_INSET_Y, INSTRUCTION_INSET_X, "Next Piece:");
	mvaddstr(INSTRUCTION_INSET_Y + 10, INSTRUCTION_INSET_X, "'a' or left:  move left");
        mvaddstr(INSTRUCTION_INSET_Y + 11, INSTRUCTION_INSET_X, "'d' or right: move right");
        mvaddstr(INSTRUCTION_INSET_Y + 12, INSTRUCTION_INSET_X, "'w' or up:    rotate");
        mvaddstr(INSTRUCTION_INSET_Y + 13, INSTRUCTION_INSET_X, "'s' or down:  drop");
        mvaddstr(INSTRUCTION_INSET_Y + 14, INSTRUCTION_INSET_X, "'q':          quit");
	mvaddstr(INSTRUCTION_INSET_Y + 16, INSTRUCTION_INSET_X, "Michael Widler, 2020");

	/* initialize game */
	clearBoard();
	setBoardBoarders();
	nextPiece();
	nextPiece();
	srand(time(0));
	setBoardBoarders();

   	/* wait for key input to start */
	mvaddstr(INSTRUCTION_INSET_Y + 8, 3, "Press Any ");
	mvaddstr(INSTRUCTION_INSET_Y + 9, 3, "  Key to  ");
	mvaddstr(INSTRUCTION_INSET_Y + 10,3, "   Begin  ");
   	while(getch() == ERR)
   	{
      		continue;
   	}

   	/* aaand go */
	refresh();
	setTimer();

	/* main game loop */
	while(TRUE)
	{
		keyPressed = getch();
 		/* ifs instead of switch because KEY_* exceed standard char value range and generate warning */
		if(keyPressed == ERR)
			continue;
		else if(keyPressed == KEY_UP || keyPressed == 'w')
			dirPressed(UP);
		else if(keyPressed == KEY_DOWN || keyPressed == 's')
			dirPressed(DOWN);
		else if(keyPressed == KEY_LEFT || keyPressed == 'a')
			dirPressed(LEFT);
		else if(keyPressed == KEY_RIGHT || keyPressed == 'd')
			dirPressed(RIGHT);
		else if(keyPressed == 'q')
		{
			exitProg();
		}
	}

	/* the unreachable return statement */
	return 0;
}

void updateScreen(void)
{
	/* early exit if game is over, otherwise there's weirdness */
	if(playerHasLost())
		return;
	setBoardContents();
	setPieceInBoard(&curPiece, curX, curY);
	wrefresh(boardWindow);
}

void applyGravity(void)
{
	if(willCollide(&curPiece, curX, curY + 1, curPiece.orientation))
	{
		affixPiece(&curPiece, curX, curY);
		nextPiece();
	}
	else
	{
		curY++;
	}
}

/* timer code from StackExchange; sets repeating timer */
void setTimer(void) {

    struct itimerval it;

    /*  Clear itimerval struct members  */

    timerclear(&it.it_interval);
    timerclear(&it.it_value);


    /*  Set timer  */

    it.it_interval.tv_usec = TIMESTEP;
    it.it_value.tv_usec    = TIMESTEP;
    setitimer(ITIMER_REAL, &it, NULL);
}

void dirPressed(int dir)
{
	/* block game input if the game is over */
	if(playerHasLost())
		return;

	if(dir == UP)
	{
		if(willCollide(&curPiece, curX, curY, getNextRotation(curPiece.orientation)) == FALSE)
                        rotate(&curPiece);
	}
	if(dir == DOWN)
	{
		applyGravity();
		applyGravity();
	}
	if(dir == LEFT)
	{
		if(willCollide(&curPiece, curX - 1, curY, curPiece.orientation) == FALSE)
			curX--;
	}
        if(dir == RIGHT)
        {
                if(willCollide(&curPiece, curX + 1, curY, curPiece.orientation) == FALSE)
                        curX++;
        }
	updateScreen();
}

void sigalrmHandler(int sig)
{
        if(playerHasLost())
        {
                mvaddstr(15, 2, " Game Over! ");
		refresh();
        }
	else
	{
		applyGravity();
		clearCompletedRows();
	        updateScreen();
	}
}



void sigintHandler(int sig)
{
	exitProg();
}

void nextPiece()
{
	curX = 4;
	curY = -3;
	setPiece(&curPiece, nextPieceInt, 0);
	nextPieceInt = rand() % 7;
	drawNextPiece();
}

void drawNextPiece(void)
{
	piece_t upcoming;
	int x = 0;
	int y = 0;
	setPiece(&upcoming, nextPieceInt, 0);
	for(x = 0; x < 4; x++)
	for(y = 0; y < 4; y++)
	{
		if(occupies(&upcoming, x, y))
			mvaddch(INSTRUCTION_INSET_Y + 1 + y, INSTRUCTION_INSET_X + 4 + x, '@');
		else
			mvaddch(INSTRUCTION_INSET_Y + 1 + y, INSTRUCTION_INSET_X + 4 + x, ' ');


	}
	wrefresh(nextPieceWindow);
}

/* put the screen back to normal params on exit */
void exitProg(void)
{
	endwin();
	exit(0);
}
