CC = gcc
CFLAGS  = -Wall
SRCS = *.c
LDFLAGS = -lncursesw
EXE=solitaire.exe

all:
	$(CC) $(CFLAGS) -o $(EXE) $(SRCS) $(LDFLAGS) 

clean:
	rm -f solitaire.exe