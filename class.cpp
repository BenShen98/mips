#include <iostream>

typedef unsigned char Regidx;
typedef unsigned int Mem;


class simulator{
	public:

		void run();
		simulator();
		~simulator();

	private:
		int reg[34]={0};//HI/LO only access by mfhi/mfhlo
		//TODO register zero = 0 !
		Mem * memInstruction;
		int PC=0x10000000;

		Mem getInstruction();

		void Rswitch();

		void ISAexception();

		void Mathexception();/* code */
 }

		void Memexception();

		void add(Regidx d,Regidx s,Regidx t);

		void addu(Regidx d,Regidx s,Regidx t);
		void jr(Regidx s);

		Mem getReg(Regidx i);
		Mem getHI();
		Mem getLO();
		void setReg(Regidx i, Mem value);
		reg[i]=5;
};



int main()
{
	simulator s1;

	s1.run();

	return 0;
}


simulator::simulator() {
	 memInstruction = new Mem[0x10000](); //0x1000000>>2
	 memInstruction[0]=0b00000000001000100001100000100001;
	 memInstruction[1]=0b00000000000000000000000000001000;
 }

 simulator::~simulator() {
 	 delete memInstruction;
  }

void simulator::run() {
	//TODO: if
	while (PC!=0) {
		switch((getInstruction())>>26){
			case 0b000000:
				Rswitch();break;
			//other case here
			default: ISAexception();break;
		}
	}
	std::cout<<"Function terminate without error!\n";
}



void simulator::Rswitch(){
	Mem const instruction=getInstruction();
	Regidx s, t, d;
	unsigned char shift;

	s=(instruction&0x03E00000) >>21;
	t=(instruction&0x001F0000) >>16;
	d=(instruction&0x0000F800) >>11;
	shift=(instruction&0x000007C0) >>6;

	switch(instruction&0x3F){
		case 0b100000: add(d,s,t);PC+=4;break;
		case 0b100001: addu(d,s,t);PC+=4;break;
		case 0b001000: jr(s);break; //no +4
	default: ISAexception();
	}
}

void simulator::add(Regidx d,Regidx s,Regidx t){
	// unsigned overflow
	// BUG ???
	int temp=reg[s]+reg[t];
	if((int(reg[s])>0 && int(reg[t])>0 && (temp&0x80000000)) || (int(reg[s])<0 && int(reg[t])<0 && !(temp&0x80000000))){
		Mathexception();
	}
	reg[d]=temp;
	std::cerr<<"add\t| "<<reg[d]<<" is result at PC "<<std::hex<<PC<<"\n";
}

void simulator::addu(Regidx d,Regidx s,Regidx t){
	//unsigned overflow
	long temp=long(reg[s])+long(reg[t]);
	if((temp>>32)){
		Mathexception();
	}

	reg[d]=temp;
	std::cerr<<"addu\t| "<<reg[d]<<" is result at PC "<<std::hex<<PC<<"\n";
}
void simulator::jr(Regidx s){
	PC=reg[s];
	std::cerr<<"jr\t| jump to memory address "<<std::hex<<PC<<"\n";
}

void simulator::ISAexception(){
	std::cerr<<"ISA exception at memory address "<<std::hex<<PC<<std::endl;
	//debug info eg PC counter....
	std::exit (-12);
}

void simulator::Mathexception(){
	std::cerr<<"Math exception at memory address "<<std::hex<<PC<<std::endl;
	//debug info eg PC counter....
	std::exit (-10);
}
void simulator::Memexception(){
	std::cerr<<"Mem exception at memory address "<<std::hex<<PC<<std::endl;
	//debug info eg PC counter....
	std::exit (-11);
}

Mem simulator::getInstruction(){

	if(PC<0x10000000 || PC>0x11000000){
		Memexception();
	}

	return memInstruction[(PC-0x10000000)>>2];
}


//bin/mips_simulator x.bin --ext-memory=memo.bin --ext-reg=reg.bin
