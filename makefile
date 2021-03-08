all: myhie worker

myhie: myhie.o sorters.o
	g++ myhie.o sorters.o -o myhie
worker: worker.o sorters.o
	g++ worker.o sorters.o -o worker
sorters.o: sorters.cpp sorters.h
	g++ -c -ggdb3 sorters.cpp -o sorters.o
worker.o: worker.cpp
	g++ -c -ggdb3 worker.cpp -o worker.o
myhie.o: myhie.cpp
	g++ -c -ggdb3 myhie.cpp -o myhie.o
clean:
	rm *.o myhie worker