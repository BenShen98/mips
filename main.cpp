#include <iostream>
#include "Simulator.hpp"
// typedef unsigned char Regidx;
// typedef unsigned int Word;

int main()
{
	Simulator s1;

	s1.run();

	//Simulator should handel the termination with exist code
	//comand below should never get run
	return -20;
}



//bin/mips_simulator x.bin --ext-memory=memo.bin --ext-reg=reg.bin
