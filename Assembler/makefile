CFLAGS=-Wall
CC=gcc
OBJS=loader.o util.o assembler.o

y86as: $(OBJS)
	$(CC) $(CFLAGS) -o $@ y86as.c $(OBJS)

loader.o:
	$(CC) $(CFLAGS) -c loader.c

assembler.o: util.o
	$(CC) $(CFLAGS) -c assembler.c

util.o:
	$(CC) $(CFLAGS) -c util.c

clean:
	rm y86as *.o
