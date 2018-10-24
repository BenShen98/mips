#include "Memory.hpp"
#include <iostream>

Word Memory::getInstruction(Word PC){
  memInstruction[0]=0b000000000010001000010000100001;

	if(PC<0x10000000 || PC>0x11000000){
		Memexception(PC);
	}

	return memInstruction[(PC-0x10000000)>>2];
}

void Memory::Memexception(Word PC){
	std::cerr<<"Mem exception at memory address "<<std::hex<<PC<<std::endl;
	//debug info eg PC counter....
	std::exit (-11);
}

Memory::Memory(){
	memInstruction[0]=0b00000000001000100001100000100001;
	memInstruction[1]=0b00000000000000000000000000001000;
}
