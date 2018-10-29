#include "Memory.hpp"
#include <iostream>
#include <fstream>

//debug
#include <bitset>


Word Memory::getInstruction(Word PC){
  //TODO: test const ins memory with line below
  // memInstruction[0]=0b000000000010001000010000100001;

	if(PC<0x10000000 || PC>=0x11000000){
		Memexception(PC);
	}

	return memInstruction[(PC-0x10000000)>>2];
}

void Memory::Memexception(Word addr){
	std::cerr<<"Mem exception at memory address "<<std::hex<<addr<<std::endl;
	//debug info eg PC counter....
	std::exit (-11);
}

// if addr is not divisiable by 4, reminder of addr is ignored
Word Memory::readWord(Word addr){
	if(addr<0x11000000 && addr>=0x10000000){
		return memInstruction[(addr-0x10000000)>>2];
	}else if(addr<0x24000000 && addr>=0x10000000 ){
		return memRW[(addr-0x20000000)>>2];
	}else if((addr>>2) == (0x30000000>>2)){
		return GETC();
	}else{
		Memexception(addr);
	}
}

Word Memory::GETC(){
	//although EOF is -1, but just to make sure
	Word temp=std::getchar();
	if (temp==EOF){
		return Word(-1);
	}else{
		return temp;
	}

}

void Memory::PUTC(Word wd){
	// putchar function convert integer unsigned char
	// https://www.programiz.com/cpp-programming/library-function/cstdio/putchar
	std::putchar(wd);
}

void Memory::writeWord(Word addr, Word wd){
	if(addr<0x24000000 && addr>=0x10000000 ){
		memRW[(addr-0x20000000)>>2]=wd;
	}else if((addr>>2) == (0x30000004>>2)){
		PUTC(wd);
	}else{
		//write to instruction address is not valid
		Memexception(addr);
	}
}

void Memory::_printInst(Word PC){
  Word inst=getInstruction(PC);

  std::cerr << "mem[0x"<<std::hex<<PC<<"]\t";

	std::cerr <<std::bitset<8>((inst&0xff000000)>>24) <<" " <<std::bitset<8>((inst&0x00ff0000)>>16)<<" " <<std::bitset<8>((inst&0x0000ff00)>>8) <<" "<<std::bitset<8>((inst&0x000000ff)) << '\n';

  // if( ((getInstruction(PC) )>>26)==0b000000 ){
  //   //R type
  //   std::cerr << std::bitset<6>(inst>>26) << " "
  //   << std::bitset<5>((inst&0x03E00000) >>21) << " "
  //   << std::bitset<5>((inst&0x001F0000) >>16) << " "
  //   << std::bitset<5>((inst&0x0000F800) >>11) << " "
  //   << std::bitset<5>((inst&0x000007C0) >>6) << " "
  //   << std::bitset<6>((inst&0x0000003f));
	//
	//
  // }else if( ((getInstruction(PC) )>>27)==0b1 ){
  //   //J type
  //   std::cerr << std::bitset<6>(inst>>26) <<" 0x"
  //   << std::hex<<(inst&0x03ffffff);
	//
  // }else{
  //   //I type
  //   std::cerr << std::bitset<6>(inst>>26) << " "
  //   << std::bitset<5>((inst&0x03E00000) >>21) << " "
  //   << std::bitset<5>((inst&0x001F0000) >>16) << " "
  //   << std::bitset<16>((inst&0x0000Ffff) >>6);
  // }
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
			// DONT edit (pitfall caused by type cast of char and int)
			UWord a,b,c,d;
		 a= (unsigned char)memblock[i];
		 b= (unsigned char)memblock[i+1];
		 c= (unsigned char)memblock[i+2];
		 d= (unsigned char)memblock[i+3];
		 std::cerr << std::bitset<8>(a) << " " << std::bitset<8>(b) << " " << std::bitset<8>(c) << " " << std::bitset<8>(d) << '\n';
			memInstruction[insIdx]=(a<<24) | (b<<16) | (c<<8) | d;
			insIdx++;
		}

    std::cerr << "below are the instruction memory inputx" << '\n';
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
