#include "Simulator.hpp"
#include <iostream>
#include <bitset>
Simulator::Simulator(char* instructionFile) {

	mem = new Memory(instructionFile);

	reg = new Register();
}

Simulator::~Simulator() {
	delete mem;
	delete reg;
}

void Simulator::run() {
	//TODO: if
	while (PC!=0) {
		UWord const instruction=mem->getInstruction(PC);
		Regidx s, t;
		s=(instruction&0x03E00000) >>21;
		t=(instruction&0x001F0000) >>16;
		UWord immediate = (instruction&0xFFFF);

		// std::cerr << (( mem->getInstruction(PC) )>>26) << '\n';
		switch(instruction>>26){
			case 0b000000:
			Rswitch();break;
			case 0b001000:
			addImm(t,s,immediate);PC+=4;break;
			case 0b001001:
			addImmUnsigned(t,s,immediate);PC+=4;break;
			case 0b001100:
			ANDI(t,s,immediate);PC+=4;break;
			case 0b001110:
			XORI(t,s,immediate);PC+=4;break;
			case 0b001101:
			ORI(t,s,immediate);PC+=4;break;
			case 0b000100:
			beq(t,s,immediate);break;
			case 0b000001:
			BranchSwitch();break;
			// case 0b000111:
			// bgtz();PC+=4;break;
			// case 0b000110:
			// blez();PC+=4;break;
			case 0b100000:
			std::cerr << "/* message */" << '\n';
			loadbyte(t,s,immediate);PC+=4;break;
			case 0b001111:
			loadupperImm(t,immediate);PC+=4;break;
			case 0b100011:
			loadword(t,s,immediate);PC+=4;break;
			case 0b101000:
			storebyte(t,s,immediate);PC+=4;break;
			case 0b101011:
			storeword(t,s,immediate);PC+=4;break;
			case 0b001010:
			setlessthan_Imm_signed(t,s,immediate);PC+=4;break;
			case 0b001011:
			setlessthan_Imm_Usigned(t,s,immediate);PC+=4;break;
			default:
			std::cerr << std::bitset<32>( instruction >>26) << '\n';
			ISAexception();break;

		}
	}
	std::cerr<<"Function terminate without error!\n";

  std::exit ( reg->get(2) &0xFF ); //return only the low 8-bits of the value in register $2
}

void Simulator::Rswitch(){
	UWord const instruction=mem->getInstruction(PC);
	Regidx s, t,d;
	unsigned char shift;
	s=(instruction&0x03E00000) >>21;
	t=(instruction&0x001F0000) >>16;
	d=(instruction&0xF800) >>11;
	shift=(instruction&0x7C0) >>6;

	switch(instruction&0x3F){
		case 0b100000: add(d,s,t);PC+=4;break;
		case 0b100001: addu(d,s,t);PC+=4;break;
		case 0b100010: sub(d,s,t);PC+=4;break;
		case 0b100011: subu(d,s,t);PC+=4;break;
		case 0b011000: multiply(s,t);PC+=4;break;
		case 0b011001: multiplyunsigned(s,t);PC+=4;break;
		case 0b001000: jr(s);break; //no +4
		case 0b100100: andbitwise(d,s,t);PC+=4;break;
		case 0b100101: orbitwise(d,s,t);PC+=4;break;
		case 0b100110: xorbitwise(d,s,t);PC+=4;break;
		case 0b010000: mfhi(d);PC+=4;break;
		case 0b010010: mflo(d);PC+=4;break;
		case 0b000000: LLshift(shift,t,d);PC+=4;break;
		case 0b000100: shiftLLVar(d,s,t);PC+=4;break;
		case 0b101010: setlt(d,s,t);PC+=4;break;
		case 0b101011: setltu(d,s,t);PC+=4;break;
		case 0b000011: shiftRA(shift,d,t);PC+=4;break;
		case 0b000010: shiftRL(shift,d,t);PC+=4;break;
		case 0b000110: shiftRVar(d,t,s);PC+=4;break;
		case 0b011011: divideunsigned(s,t);PC+=4;break;
		default: ISAexception();
	}
}

void Simulator::BranchSwitch(){
	UWord const instruction=mem->getInstruction(PC);
	Regidx s;
	UWord immediate = (instruction&0xFFFF);
	s=(instruction&0x03E00000) >>21;

	switch (instruction&0x1F0000){
		case 0b00001:
		bgez(s,immediate);PC+=4;break;
		// case 0b10001:
		// bgezal();PC+=4;break;
		// case 0b00000:
		// bltz();PC+=4;break;
		// case 0b10000:
		// bltzal();PC+=4;break;
	}
}

void Simulator::add(Regidx d,Regidx s,Regidx t){
	// unsigned overflow
	// BUG ???
	int temp=reg->get(s)+reg->get(t);
	if((Word(reg->get(s))>0 && Word(reg->get(t))>0 && (temp&0x80000000)) || (Word(reg->get(s))<0 && Word(reg->get(t))<0 && !(temp&0x80000000))){
		Mathexception();
	}
	reg->set(d,temp);
	std::cerr<<"add\t| "<<std::dec<<reg->get(d)<<" is result at PC "<<std::hex<<PC<<"\n";
}

void Simulator::sub(Regidx d,Regidx s,Regidx t){
	//rs - rt => d
	//postive - postive => NO overflow possible
	//postive (0 INCLUSIVE) - negative => overflow if get negative 	******
	//negative - postive => overflow if get postive									******
	//negative - negative => NO overflow ???
	//0 - most negative => most negative (overflow)
	Word rs,rt;
	rs=Word(reg->get(s));
	rt=Word(reg->get(t));
	Word temp=rs-rt;
	if( (rs>=0 && rt<0 && temp<0) || (rs<0 && rt>0 && temp>0) ){
		Mathexception();
	}

	reg->set(d,temp);

	std::cerr<<"sub\t| "<<std::dec<<reg->get(d)<<" is result at PC "<<std::hex<<PC<<"\n";
}

void Simulator::subu(Regidx d,Regidx s,Regidx t){
	//no exceptions here checked with PDF
	reg->set(d,UWord(reg->get(s))-UWord(reg->get(t)));
}

void Simulator::addu(Regidx d,Regidx s,Regidx t){
	//unsigned overflow
	unsigned int temp = UWord(reg->get(s)) + UWord(reg->get(t));

	//no exception handling required,overflow should be ignored

	reg->set(d,temp);
	std::cerr<<"addu\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::jr(Regidx s){
	PC=reg->get(s);
	std::cerr<<"jr\t| jump to memory address 0x"<<std::hex<<PC<<"\n";
}

void Simulator::andbitwise(Regidx d,Regidx s,Regidx t){
	int temp = reg->get(s) & reg->get(t);
	reg->set(d,temp);
	std::cerr<<"and\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::orbitwise(Regidx d,Regidx s,Regidx t){
	int temp = reg->get(s) | reg->get(t);
	reg->set(d,temp);
	std::cerr<<"oru\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::xorbitwise(Regidx d,Regidx s,Regidx t){
	int temp = reg->get(s) ^ reg->get(t);
	reg->set(d,temp);
	std::cerr<<"xor\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::mfhi(Regidx d ){
	reg->set(d,reg->getHI());
	std::cerr<<"mfhi\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::mflo(Regidx d ){
	reg->set(d,reg->getLO());
	std::cerr<<"mflo\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::multiply(Regidx s,Regidx t){
	//no exception here checked with PDF
	signed long temp = (signed long)(reg->get(s)*(signed long)(reg->get(t)));
	reg->setLO(temp&0xFFFFFFFF);
	reg->setHI(UWord(temp>>32));
	std::cerr<<"multiplysigned\t| "<<std::dec<<reg->getLO()<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::multiplyunsigned(Regidx s,Regidx t){
	//no exception here checked with PDF
	long temp = long(reg->get(s))*long(reg->get(t));
	reg->setLO(temp&0xFFFFFFFF);
	reg->setHI(UWord(temp>>32));
	std::cerr<<"multiplyunsigned\t| "<<std::dec<<reg->getLO()<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::divideunsigned(Regidx s, Regidx t){
	//no exceptions here checked with PDF
	reg->setLO(reg->get(s)/reg->get(t));
	reg->setHI(reg->get(s)%reg->get(t));
}

void Simulator::LLshift(unsigned char shift,Regidx t, Regidx d){
	long temp = (long(reg->get(t))<<shift);
	//no exception detection for unsigned op
	reg->set(d,(int)temp);
	std::cerr<<"shiftLL\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::shiftLLVar(Regidx d , Regidx s, Regidx t){
	long temp = (long(reg->get(t))<<reg->get(s));
	if (temp>>32){
		Mathexception();
	}
	reg->set(d,(int)temp);
	std::cerr<<"shiftLLVar\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::setlt(Regidx d , Regidx s, Regidx t){
	if(((signed int)reg->get(s))<((signed int)reg->get(t))){
		reg->set(d,1);
	}
	else {
		reg->set(d,0);
	}
	//TODO might have exception
	std::cerr<<"setlt\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::setltu(Regidx d , Regidx s, Regidx t){
	if(((signed int)reg->get(s))<((signed int)reg->get(t))){
		reg->set(d,1);
	}
	else {
		reg->set(d,0);
	}
	//TODO might have exception
	std::cerr<<"setltu\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::shiftRA(unsigned char shift,Regidx d,Regidx t){
	if (((signed int)reg->get(t))>=0){
		reg->set(d,(reg->get(t)>>shift));
	}
	else{
		int power=1;
		power=power<<shift;
		reg->set(d,((signed int)reg->get(t)/power));
	}
	std::cerr<<"shiftRArithmitic\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::shiftRL(unsigned char shift,Regidx d,Regidx t){
	reg->set(d,(reg->get(t)>>shift));
	//TODO might have exception here
	std::cerr<<"shiftRL\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::shiftRVar(Regidx d,Regidx t,Regidx s){
	//BUG here sequence not consistent with other functions
	reg->set(d,(reg->get(t)>>reg->get(s)));
	//TODO might have exception here
	std::cerr<<"shiftRVar\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

//---------------------------------**********  I Type functions  **********------------------------------

void Simulator::addImm(Regidx t,Regidx s, Word immediate){
	if (immediate & 0x8000){
		immediate = (immediate | (0xFFFF0000));
		//std::cerr << "immediate" <<std::bitset<32>(immediate)<< '\n';
	}
	Word temp=Word(reg->get(s))+Word(immediate);
	if((Word(reg->get(s))>0 && Word(immediate>0) && (temp&0x80000000)) || (Word(reg->get(s))<0 && Word(immediate<0) &&  !(temp&0x80000000))){
		Mathexception();
	}
	reg->set(t,temp);
	std::cerr<<"addImm\t| "<<std::dec<<reg->get(t)<<" is result at PC "<<std::hex<<PC<<"\n";
}

void Simulator::addImmUnsigned(Regidx t,Regidx s,UWord immediate){
	//unsigned overflow
	UWord temp = (UWord) (reg->get(s)) + immediate;

	//no exception handling required,overflow should be ignored

	reg->set(t,temp);
	std::cerr<<"addImmUnsigned\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::ANDI(Regidx t,Regidx s,UWord immediate){
	int temp = reg->get(s) & immediate;
	reg->set(t,temp);
	std::cerr<<"ANI\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::XORI(Regidx t,Regidx s,UWord immediate){
	int temp = reg->get(s) ^ immediate;
	reg->set(t,temp);
	std::cerr<<"XORI\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::ORI(Regidx t,Regidx s,UWord immediate){
	int temp = reg->get(s) | immediate;
	reg->set(t,temp);
	std::cerr<<"ORI\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::beq(Regidx t,Regidx s,Word immediate){
	//BUG NO IMPLEMENTATION OF DELAY BRANCH !!
	if (reg->get(t)==reg->get(s)){
		PC+=immediate<<2;
	}
	else{
		PC+=4;
	}
}

void Simulator::bgez(Regidx s,Word immediate){
	//BUG NO IMPLEMENTATION OF DELAY BRANCH !!
	if ((Word)reg->get(s)>=0){
		PC+=immediate<<2;
	}
	else{
		PC+=4;
	}
}

void Simulator::bgezal(Regidx s,Word immediate){
	//BUG NO IMPLEMENTATION OF DELAY BRANCH !!
	if ((Word)reg->get(s)>=0){
		PC+=immediate<<2;
	}
	else{
		PC+=4;
	}
}

void Simulator::loadbyte(Regidx t, Regidx s,Word immediate){
	if (immediate & 0x8000){
		immediate = (immediate | (0xFFFF0000));
	}
	std::cerr << std::bitset<32>(immediate) << '\n';

	//signed extend it otherwise no change
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	std::cerr << "byteAddr" <<std::dec << byteAddr << '\n';
	Word temp=mem->readWord(byteAddr);
	std::cerr << "word temp" <<std::dec << temp << '\n';
	Word result = word2Sbyte(temp,byteAddr%4);

	reg->set(t,result);
	std::cerr<<"loadbyte\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

inline signed char Simulator::word2Sbyte(Word word, int idx){
	Word temp=word >> (8*(3-idx) ) & 0xFF;
	std::cerr << "byte from word2byte"<<temp << '\n';
	return (signed char)(temp);
}

void Simulator::storebyte(Regidx t, Regidx s,Word immediate){
	if (immediate & 0x8000){
		immediate = (immediate | (0xFFFF0000));
	}
	std::cerr << std::bitset<32>(immediate) << '\n';

	//signed extend it otherwise no change
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	std::cerr << "byteAddr" <<std::dec << byteAddr << '\n';
	Word temp=mem->readWord(byteAddr);
	std::cerr << "word temp" <<std::dec << temp << '\n';
	Word result = word2Sbyte(temp,byteAddr%4);

	reg->set(t,result);
	std::cerr<<"storebyte\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::loadword(Regidx t, Regidx s,Word immediate){
	if (immediate & 0x8000){
		immediate = (immediate | (0xFFFF0000));
	}
	std::cerr << std::bitset<32>(immediate) << '\n';
	//check immediate overflow
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	std::cerr << "byteAddr" <<std::dec << byteAddr << '\n';
	Word temp=mem->readWord(byteAddr);
	std::cerr << "word temp" <<std::dec << temp << '\n';
	reg->set(t,temp);
	std::cerr<<"loadword\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::storeword(Regidx t, Regidx s,Word immediate){
	if (immediate & 0x8000){
		immediate = (immediate | (0xFFFF0000));
	}
	std::cerr << std::bitset<32>(immediate) << '\n';
	//check immediate overflow
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	std::cerr << "byteAddr" <<std::dec << byteAddr << '\n';
	mem->writeWord(byteAddr,reg->get(t));
	std::cerr<<"storeword\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::loadupperImm(Regidx t,UWord immediate){
	Word temp=Word(immediate<<16);
	reg->set(t,temp);
}

void Simulator::setlessthan_Imm_signed(Regidx t, Regidx s, Word immediate){
	if (immediate & 0x8000){
		immediate = (immediate | (0xFFFF0000));
	}

	if (Word(reg->get(s))<Word(immediate)){
		reg->set(t,1);
	}
	else {
		reg->set(t,0);
	}
	std::cerr<<"setlessthan_Imm_signed\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::setlessthan_Imm_Usigned(Regidx t, Regidx s, UWord immediate){
	if (UWord(reg->get(s))<UWord(immediate)){
		reg->set(t,1);
	}
	else {
		reg->set(t,0);
	}
	std::cerr<<"setlessthan_Imm_Usigned\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

//**********************************  Exception functions *****************************************************

void Simulator::ISAexception(){
	std::cerr<<"ISA exception at memory address 0x"<<std::hex<<PC<<std::endl;
	//debug info eg PC counter....
	std::exit (-12);
}

void Simulator::Mathexception(){
	std::cerr<<"Math exception at memory address 0x"<<std::hex<<PC<<std::endl;
	//debug info eg PC counter....
	std::exit (-10);
}
