CC = gcc
CFLAGS  = -Wall
SRCS = *.c
LDFLAGS = -lncurses
EXE=solitaire.exe

all:
	$(CC) $(CFLAGS) -o $(EXE) $(SRCS) $(LDFLAGS) 

clean:
	rm -f solitaire.exe