CC = gcc
CFLAGS = -Wall

chell: chell.o lineedit.o exec.o
	$(CC) $(CFLAGS) -o chell chell.o lineedit.o exec.o

chell.o: chell.c
	$(CC) $(CFLAGS) -c chell.c

lineedit.o: lineedit.c
	$(CC) $(CFLAGS) -c lineedit.c

exec.o: exec.c
	$(CC) $(CFLAGS) -c exec.c

clean:
	rm -f *.o chell
