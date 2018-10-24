#include "Memory.hpp"
#include <iostream>
#include <fstream>

//debug
#include <bitset>


Word Memory::getInstruction(Word PC){
  //TODO: test const ins memory with line below
  // memInstruction[0]=0b000000000010001000010000100001;

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

Memory::Memory(char* instructionFile){
	std::cerr << "generating instruction memory" << '\n';

	std::streampos size;
	char * memblock;

	std::ifstream file (instructionFile, std::ios::in|std::ios::binary|std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, std::ios::beg);
		file.read (memblock, size);
		file.close();

		int insIdx=0;

		for (int i=0; i<size;i=i+4){
			//big endian
			Word instruct=memblock[3];
			instruct = ((memblock[i+2])<<8) | instruct;
			instruct = ((memblock[i+1])<<16) | instruct;
			instruct = ((memblock[i])<<24) | instruct;
			memInstruction[insIdx]=instruct;
			std::cerr << "at ins idx "<<insIdx<<"|"<<std::bitset<32>(memInstruction[insIdx]) << '\n';
			insIdx++;
		}

		delete[] memblock;
	}else{
		std::cerr << "unable to read instruction bin" << '\n';
		std::exit(-20);
	}

}
