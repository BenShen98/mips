#include "Simulator.hpp"
#include <iostream>

Simulator::Simulator() {

	 mem = new Memory();

	 reg = new Register();
 }

 Simulator::~Simulator() {
 	 delete mem;
	 delete reg;
  }

void Simulator::run() {
	//TODO: if
	while (PC!=0) {
		switch(( mem->getInstruction(PC) )>>26){
			case 0b000000:
				Rswitch();break;
			//other case here
			default: ISAexception();break;
		}
	}
	std::cout<<"Function terminate without error!\n";

	reg->set(2,0xaa);
  std::exit ( reg->get(2) &0xFF ); //return only the low 8-bits of the value in register $2
}



void Simulator::Rswitch(){
	Word const instruction=mem->getInstruction(PC);
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

void Simulator::add(Regidx d,Regidx s,Regidx t){
	// unsigned overflow
	// BUG ???
	int temp=reg->get(s)+reg->get(t);
	if((int(reg->get(s))>0 && int(reg->get(t))>0 && (temp&0x80000000)) || (int(reg->get(s))<0 && int(reg->get(t))<0 && !(temp&0x80000000))){
		Mathexception();
	}
	reg->set(d,temp);
	std::cerr<<"add\t| "<<reg->get(d)<<" is result at PC "<<std::hex<<PC<<"\n";
}

void Simulator::addu(Regidx d,Regidx s,Regidx t){
	//unsigned overflow
	long temp=long(reg->get(s))+long(reg->get(t));
	if((temp>>32)){
		Mathexception();
	}

	reg->set(d,temp);
	std::cerr<<"addu\t| "<<reg->get(d)<<" is result at PC "<<std::hex<<PC<<"\n";
}

void Simulator::jr(Regidx s){
	PC=reg->get(s);
	std::cerr<<"jr\t| jump to memory address "<<std::hex<<PC<<"\n";
}

void Simulator::andbitwise(Regidx d,Regidx s,Regidx t){
	int temp = reg->get(s) & reg->get(t);
	reg->set(d,temp);
}

void Simulator::orbitwise(Regidx d,Regidx s,Regidx t){
	int temp = reg->get(s) | reg->get(t);
	reg->set(d,temp);
}

void Simulator::xorbitwise(Regidx d,Regidx s,Regidx t){
	int temp = reg->get(s) ^ reg->get(t);
	reg->set(d,temp);
}

void mfhi(Regidx d ){
	reg->set(d,reg->getHI());
}

void mflo(Regidx d ){
	reg->set(d,reg->getLO());
}

void multiply(Regidx s,Regidx t){
	int temp=reg->get(s)*reg->get(t);
	if ()
}

void Simulator::ISAexception(){
	std::cerr<<"ISA exception at memory address "<<std::hex<<PC<<std::endl;
	//debug info eg PC counter....
	std::exit (-12);
}

void Simulator::Mathexception(){
	std::cerr<<"Math exception at memory address "<<std::hex<<PC<<std::endl;
	//debug info eg PC counter....
	std::exit (-10);
}
