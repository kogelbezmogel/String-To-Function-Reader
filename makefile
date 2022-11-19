all: compile link clean

compile:
	g++ -std=c++17 -c main.cpp AnaliticFunctionReader.cpp BasicFunctions.cpp

link:
	g++ -std=c++17 main.o AnaliticFunctionReader.o BasicFunctions.o -o main

clean:
	rm -f *.o
