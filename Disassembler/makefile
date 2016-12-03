CFLAGS=-Wall
CC=gcc
OBJS=loader.o tokenizer.o util.o disassembler.o

y86dis: $(OBJS)
	$(CC) $(CFLAGS) -o $@ y86dis.c $(OBJS)

loader.o: tokenizer.o
	$(CC) $(CFLAGS) -c loader.c

disassembler.o: util.o
	$(CC) $(CFLAGS) -c disassembler.c

tokenizer.o:
	$(CC) $(CFLAGS) -c tokenizer.c

util.o:
	$(CC) $(CFLAGS) -c util.c

clean:
	rm y86dis *.o
