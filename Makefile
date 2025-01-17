all: main.c mmm.o rtclock.o
	gcc -O2 -O3 -march=native -Wall -g -o mmm main.c mmm.o rtclock.o -lpthread -lm

mmm.o: mmm.h mmm.c
	gcc -Wall -g -c mmm.c

rtclock.o: rtclock.h rtclock.c
	gcc -Wall -g -c rtclock.c

clean:
	rm -f mmm *.o
