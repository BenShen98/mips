all: Register.o Memory.o main.o
	g++ Register.o Memory.o main.o -o prog

Register.o: Register.cpp Register.hpp
	g++ -c Register.cpp -std=c++11

Memory.o: Memory.cpp Memory.hpp
	g++ -c Memory.cpp -std=c++11

main.o: main.cpp
	g++ -c main.cpp -std=c++11


run: all
	./prog
