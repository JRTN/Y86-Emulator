CFLAGS=-Wall
CC=gcc
OBJS=loader.o tokenizer.o util.o assembler.o

y86as: $(OBJS)
	$(CC) $(CFLAGS) -o $@ y86as.c $(OBJS)

loader.o: tokenizer.o
	$(CC) $(CFLAGS) -c loader.c

assembler.o: util.o
	$(CC) $(CFLAGS) -c assembler.c

tokenizer.o:
	$(CC) $(CFLAGS) -c tokenizer.c

util.o:
	$(CC) $(CFLAGS) -c util.c

clean:
	rm y86dis *.o
