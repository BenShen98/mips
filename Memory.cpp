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

void Memory::_printInst(Word PC){
  Word inst=getInstruction(PC);

  std::cerr << "mem[0x"<<std::hex<<PC<<"]\t";

  if( ((getInstruction(PC) )>>26)==0b000000 ){
    //R type
    std::cerr << std::bitset<6>(inst>>26) << " "
    << std::bitset<5>((inst&0x03E00000) >>21) << " "
    << std::bitset<5>((inst&0x001F0000) >>16) << " "
    << std::bitset<5>((inst&0x0000F800) >>11) << " "
    << std::bitset<5>((inst&0x000007C0) >>6) << " "
    << std::bitset<6>((inst&0x0000003f));


  }else if( ((getInstruction(PC) )>>27)==0b1 ){
    //J type
    std::cerr << std::bitset<6>(inst>>26) <<" 0x"
    << std::hex<<(inst&0x03ffffff);

  }else{
    //I type
    std::cerr << std::bitset<6>(inst>>26) << " "
    << std::bitset<5>((inst&0x03E00000) >>21) << " "
    << std::bitset<5>((inst&0x001F0000) >>16) << " "
    << std::bitset<16>((inst&0x0000Ffff) >>6);
  }
std::cerr << std::endl;
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
			insIdx++;
		}

    std::cerr << "below are the instruction memory input" << '\n';
    for (int i=0;i<insIdx;i++){
      _printInst((i<<2)+0x10000000);
    }

    // while(1){
    //
    // }

		delete[] memblock;
	}else{
		std::cerr << "unable to read instruction bin" << '\n';
		std::exit(-20);
	}

}
