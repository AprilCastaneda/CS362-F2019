CFLAGS = -Wall -fpic -coverage -lm -std=c99

default: test.out

test.out: testme.c
	
	echo "Result for running test:" > test.out
	echo "testme.c:" >> test.out
	gcc -o testme testme.c $(CFLAGS)
	./testme >> test.out 
	
	echo "Running gcov on branches:" > test.out
	gcov testme.c -b >> test.out

clean:
	rm test.out

cleanall: 
	rm test.out testme *.gcda *.gcno *.gcov
