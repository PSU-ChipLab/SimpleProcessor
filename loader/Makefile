CC           = gcc
OPTIMIZATION = -O2 -DNDEBUG
DEBUG        = -O0 -g -ggdb

BASE_OPT     = -pedantic-errors -ansi -std=c89 -Wall -Wextra \
               -Wshadow -Wpointer-arith -Wstrict-prototypes \
               -Wmissing-prototypes -Wparentheses -Wsequence-point \
               -Wcast-align -Wconversion -Werror -Wcast-qual 

#-------------------------------------
debug:   OPT = $(DEBUG) $(BASE_OPT)
release: OPT = $(OPTIMIZATION) $(BASE_OPT)
#-------------------------------------


loader: loader.o
	$(CC) $(OPT) loader.o -o loader

loader.o: loader.c
	$(CC) $(OPT) -c loader.c -o loader.o

clean:
	rm -fr *.o
