all: sparse

CFLAGS=-c -W -Wall -O3 
#CFLAGS=-c -W -Wall -g3 

sparse: main.o
	gcc -o sparse main.o -lm

main.o: main.c
	gcc ${CFLAGS} main.c

.PHONY: all clean

clean:
	rm -f *.o sparse *.dat *.gz
