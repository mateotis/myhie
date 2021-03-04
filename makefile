all: myhie worker

myhie: myhie.o
	g++ myhie.o -o myhie
worker: worker.o
	g++ worker.o -o worker
worker.o: worker.cpp
	g++ -c worker.cpp
myhie.o: myhie.cpp
	g++ -c myhie.cpp
clean:
	rm *.o myhie worker