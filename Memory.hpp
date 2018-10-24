typedef unsigned int Word;


class Memory{

//NOTE: remember interface for this class is in unit of byte (8bit), but it is stored as word (4BYTE!)

public:
	Word getInstruction(Word PC);
  Memory();

private:
	Word  memInstruction[0x10000]={0}; //0x1000000>>2 SEE note
  // Word const  memInstruction[0x10000]={0}; //0x1000000>>2 SEE note
	Word memRW[0x40000]={0};	//0x4000000>>2 SEE note

	void Memexception(Word PC);


	//invoke with special address for external R/W
	void GETC();
	void PUTC();
};
