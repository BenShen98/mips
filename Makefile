all: Register.o Memory.o Simulator.o main.o
	g++ Register.o Memory.o main.o Simulator.o -o prog

Register.o: Register.cpp Register.hpp
	g++ -c Register.cpp -std=c++11

Memory.o: Memory.cpp Memory.hpp
	g++ -c Memory.cpp -std=c++11

main.o: main.cpp
	g++ -c main.cpp -std=c++11

Simulator.o: Simulator.cpp Simulator.hpp
	g++ -c Simulator.cpp -std=c++11

run: all
	./prog

clean:
	rm ./*.o
	rm ./prog

bin:
	g++ -std=c++11 iotest/write.cpp -o ./iotest/a.out
	./iotest/a.out
	rm ./iotest/a.out
