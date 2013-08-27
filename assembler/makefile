CC      =     gcc
OPTS    =     -ansi -pedantic -Wall -Wextra -g

default: pas clean


pas: pas.o parser.o symb.o asm.o utils.o
	$(CC) $(OPTS) -o pas utils.o symb.o asm.o pas.o parser.o

pas.o: pas.c
	$(CC) $(OPTS) -c pas.c -o pas.o

parser.o: parser.c parser.h
	$(CC) $(OPTS) -c parser.c -o parser.o
	
symb.o: symb.c symb.h
	$(CC) $(OPTS) -c symb.c -o symb.o

utils.o: utils.c utils.h
	$(CC) $(OPTS) -c utils.c -o utils.o

clean:
	rm -f *.o *~
	rm -f chips/*.o chips/*~
	
	
################################################
# ASM
asm.o: asm0.o asm1.o
	ld -r asm0.o asm1.o -o asm.o

asm0.o: chips/chip_v0.c
	$(CC) $(OPTS) -c chips/chip_v0.c -o asm0.o

asm1.o: chips/chip_v1.c
	$(CC) $(OPTS) -c chips/chip_v1.c -o asm1.o