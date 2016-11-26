CFLAGS=-Wall
CC=gcc
OBJS=loader.o architecture.o tokenizer.o util.o

y86emul: $(OBJS)
	$(CC) $(CFLAGS) -o y86emul y86emul.c $(OBJS)

architecture.o:
	$(CC) $(CFLAGS) -c architecture.c

loader.o:
	$(CC) $(CFLAGS) -c loader.c

tokenizer.o:
	$(CC) $(CFLAGS) -c tokenizer.c

util.o:
	$(CC) $(CFLAGS) -c util.c

clean:
	rm y86emul *.o
