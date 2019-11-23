CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

unittest1: unittest1.c dominion.o rngs.o
	gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o
	gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c dominion.o rngs.o
	gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c dominion.o rngs.o
	gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)

unittest5: unittest5.c dominion.o rngs.o
	gcc -o unittest5 -g  unittest5.c dominion.o rngs.o $(CFLAGS)

unittestresults: unittest1 unittest2 unittest3 unittest4 unittest5
	./unittest1 >> unittestresults.out
	./unittest2 >> unittestresults.out
	./unittest3 >> unittestresults.out
	./unittest4 >> unittestresults.out
	./unittest5 >> unittestresults.out
	gcov -b -f dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out	

randomtest1: randomtestcard1.c dominion.o rngs.o
	gcc -o randomtest1 -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)

randomtest2: randomtestcard2.c dominion.o rngs.o
	gcc -o randomtest2 -g  randomtestcard2.c dominion.o rngs.o $(CFLAGS)

randomtest3: randomtestcard3.c dominion.o rngs.o
	gcc -o randomtest3 -g  randomtestcard3.c dominion.o rngs.o $(CFLAGS)

randomtestresults: randomtest1 randomtest2 randomtest3
	./randomtest1 >> randomtestresults.out
	./randomtest2 >> randomtestresults.out
	./randomtest3 >> randomtestresults.out
	gcov -b -f dominion.c >> randomtestresults.out
	cat dominion.c.gcov >> randomtestresults.out

alltestresults: unittest1 unittest2 unittest3 unittest4 unittest5 randomtest1 randomtest2 randomtest3
	./unittest1 >> alltestresults.out
	./unittest2 >> alltestresults.out
	./unittest3 >> alltestresults.out
	./unittest4 >> alltestresults.out
	./unittest5 >> alltestresults.out
	./randomtest1 >> alltestresults.out
	./randomtest2 >> alltestresults.out
	./randomtest3 >> alltestresults.out
	gcov -b -f dominion.c >> alltestresults.out
	cat dominion.c.gcov >> alltestresults.out		

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe unittest1 unittest1.exe unittest2 unittest2.exe unittest3 unittest3.exe unittest4 unittest4.exe
