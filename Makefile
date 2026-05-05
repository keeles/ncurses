CC = gcc
NCURSES_PREFIX = $(shell brew --prefix ncurses)
CFLAGS = -Wall -Wextra -g -I$(NCURSES_PREFIX)/include
LIBS = -L$(NCURSES_PREFIX)/lib -lncurses

rogue: main.c map.c player.c
	$(CC) $(CFLAGS) -o rogue main.c map.c player.c $(LIBS)

clean:
	rm -f rogue
	rm -r rogue.dSYM
