#include "Simulator.hpp"
#include<iostream>
// typedef unsigned char Regidx;
// typedef unsigned int Word;

int main(int argc, char** argv)
{
	std::cerr << "init Simulator, trying to open "<< argv[1] << '\n';

	Simulator s1(argv[1]);

	s1.run();

	//Simulator should handel the termination with exist code
	//comand below should never get run
	return -20;
}



//bin/mips_simulator x.bin --ext-memory=memo.bin --ext-reg=reg.bin
