CC = gcc
CFLAGS = -Wall -lcurses
PROG1 = main
PROG2 = piece
PROG3 = board
EXE_NAME = tetronimo
TAR_FILE = Tetronimo.tar.gz
COMMENT = "Behold, a glorious comment."

all: $(PROG1)

$(PROG1): $(PROG1).c $(PROG2).c $(PROG3).c
	$(CC) $(CFLAGS) -o $(EXE_NAME) $(PROG1).c $(PROG2).c $(PROG3).c
	chmod og-rxw $(EXE_NAME)

clean:
	rm -f $(PROG1) $(PROG2) $(PROG3) *.o *~ \#*

tar:
	rm -f $(TAR_FILE)
	tar cvzf $(TAR_FILE) *.[ch] [Mm]akefile

comment:
	echo $(COMMENT)

opt:
	make CFLAGS=-O


