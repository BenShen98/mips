#include "Memory.hpp"
#include <iostream>

Word Memory::getInstruction(Word PC){
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
	// memInstruction[0]=0b00000000001000100001100000100001;
  memInstruction[0]=0b00000000001000110010100010000000;//this code left shifts 2
  memInstruction[1]=0b00000000000000100011000010000000;//this code left shifts another register
  memInstruction[2]=0b00000000101001100011100000100001;//add
	memInstruction[3]=0b00000000000000000000000000001000;//This is jump
}
