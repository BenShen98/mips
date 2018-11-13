#include "Simulator.hpp"
#include <iostream>
#include <bitset>
#include "helper.cpp"

typedef unsigned long Ulong;

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
		executeInstruction();
		advPC();
	}
	std::cerr<<"Function terminate without error!\n";

  std::exit ( reg->get(2) &0xFF ); //return only the low 8-bits of the value in register $2
}

inline void Simulator::executeInstruction(){
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
		addi(t,s,immediate);break;
		case 0b001001:
		addiu(t,s,immediate);break;
		case 0b001100:
		ANDI(t,s,immediate);break;
		case 0b001110:
		XORI(t,s,immediate);break;
		case 0b001101:
		ORI(t,s,immediate);break;
		case 0b000100:
		beq(t,s,immediate);break;
		case 0b000001:
		BranchSwitch();break;
		case 0b000111:
		bgtz(s,immediate);break;
		case 0b000110:
		blez(s,immediate);break;
		case 0b000101:
		bne(t,s,immediate);break;
		case 0b100000:
		lb(t,s,immediate);break;
		case 0b100100:
		lbu(t,s,immediate);break;
		case 0b001111:
		lui(t,immediate);break;
		case 0b100011:
		lw(t,s,immediate);break;
		case 0b100001:
		lh(t,s,immediate);break;
		case 0b100101:
		lhu(t,s,immediate);break;
		case 0b100010:
		lwl(t,s,immediate);break;
		case 0b100110:
		lwr(t,s,immediate);break;
		case 0b101000:
		sb(t,s,immediate);break;
		case 0b101011:
		sw(t,s,immediate);break;
		case 0b101001:
		sh(t,s,immediate);break;
		case 0b001011:
		sltiu(t,s,immediate);break;
		case 0b001010:
		slti(t,s,immediate);break;
		case 0b000010:
		j(instruction&0x3FFFFFF);break;
		case 0b000011:
		jal(instruction&0x3FFFFFF);break;
		default:
		std::cerr << std::bitset<32>( instruction >>26) << '\n';
		ISAexception();break;
	}
}

inline void Simulator::advPC(){
	if(advPCbool){
		PC+=4;
	}else{
		advPCbool=true;
	}
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
		case 0b100000: add(d,s,t);break;
		case 0b100001: addu(d,s,t);break;
		case 0b100010: sub(d,s,t);break;
		case 0b100011: subu(d,s,t);break;
		case 0b011000: mult(s,t);break;
		case 0b011001: multu(s,t);break;
		case 0b001000: jr(s);break;
		case 0b100100: andbitwise(d,s,t);break;
		case 0b100101: orbitwise(d,s,t);break;
		case 0b100110: xorbitwise(d,s,t);break;
		case 0b010000: mfhi(d);break;
		case 0b010001: mthi(s);break;
		case 0b010010: mflo(d);break;
		case 0b010011: mtlo(s);break;
		case 0b000000: sll(shift,t,d);break;
		case 0b000100: sllv(d,s,t);break;
		case 0b101010: slt(d,s,t);break;
		case 0b101011: sltu(d,s,t);break;
		case 0b000011: sra(shift,d,t);break;
		case 0b000111: srav(d,s,t);break;
		case 0b000010: srl(shift,d,t);break;
		case 0b000110: srlv(d,t,s);break;
		case 0b011010: div(s,t);break;
		case 0b011011: divu(s,t);break;
		case 0b001001: jalr(d,s);break;
		default: ISAexception();
	}
}

void Simulator::BranchSwitch(){
	UWord const instruction=mem->getInstruction(PC);
	Regidx s;
	UWord immediate = (instruction&0xFFFF);
	s=(instruction&0x03E00000) >>21;

	switch ((instruction&0x1F0000)>>16){
		case 0b00001:
		bgez(s,immediate);break;
		case 0b10001:
		std::cerr << "/* error message */" << '\n';
		bgezal(s,immediate);break;
		case 0b00000:
		bltz(s,immediate);break;
		case 0b10000:
		bltzal(s,immediate);break;
	}
}

inline void Simulator::checkSignedOverflow(Word a, Word b, Word result){
	if((a&0x80000000) && (b&0x80000000) && !(result&0x80000000) || //postive + postive => negative (overflow)
			!(a&0x80000000) && !(b&0x80000000) && (result&0x80000000)) //negative + negative => postive (overflow)
	{
				Mathexception();
	}
}

void Simulator::add(Regidx d,Regidx s,Regidx t){
	int temp=reg->get(s)+reg->get(t);
	checkSignedOverflow(reg->get(s),reg->get(t),temp);
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
	UWord temp = UWord(reg->get(s)) + UWord(reg->get(t));
	reg->set(d,temp);
	//no overflow check
	std::cerr<<"addu\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::jr(Regidx s){
	advPC();
	executeInstruction();
	PC=reg->get(s);
	advPCbool=false;
	std::cerr<<"jr\t| jump to memory address 0x"<<std::hex<<PC<<"\n";
}

void Simulator::jalr(Regidx d,Regidx s){
	advPC();
	executeInstruction();
	reg->set(d,PC+4);
	PC=reg->get(s);
	advPCbool=false;
	std::cerr<<"jalr\t| jump to memory address 0x"<<std::hex<<PC<<"\n";

}

void Simulator::andbitwise(Regidx d,Regidx s,Regidx t){
	Word temp = reg->get(s) & reg->get(t);
	reg->set(d,temp);
	//no exception
	std::cerr<<"and\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::orbitwise(Regidx d,Regidx s,Regidx t){
	int temp = reg->get(s) | reg->get(t);
	reg->set(d,temp);
	std::cerr<<"or\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::xorbitwise(Regidx d,Regidx s,Regidx t){
	Word temp = (reg->get(s)) ^ (reg->get(t));
	reg->set(d,temp);
	std::cerr<<"xor\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::mthi(Regidx s){
	reg->setHI(reg->get(s));
	std::cerr<<"mthi\t| "<<std::dec<<reg->getHI()<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::mtlo(Regidx s){
	reg->setLO(reg->get(s));
	std::cerr<<"mtlo\t| "<<std::dec<<reg->getHI()<<" is result at PC 0x"<<std::hex<<PC<<"\n";

}

void Simulator::mfhi(Regidx d ){
	reg->set(d,reg->getHI());
	std::cerr<<"mfhi\t| "<<std::dec<<reg->getHI()<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::mflo(Regidx d ){
	reg->set(d,reg->getLO());
	std::cerr<<"mflo\t| "<<std::dec<<reg->getLO()<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::mult(Regidx s,Regidx t){
	//no exception here checked with PDF
	signed long temp = (signed long)(reg->get(s)*(signed long)(reg->get(t)));
	reg->setLO(temp&0xFFFFFFFF);
	reg->setHI(UWord(temp>>32));
	std::cerr<<"multiplysigned\t| "<<std::dec<<reg->getLO()<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::multu(Regidx s,Regidx t){
	//Careful here, when type convert from SIGNED int -> UNSIGNED long, C++ will do sign extension, use type cast to prevent it
	Ulong a,b,temp;
	a=UWord(reg->get(s));
	b=UWord(reg->get(t));
	temp=a*b;
	std::cerr << std::hex<<temp << '\n';
	reg->setLO(temp&0xFFFFFFFF);
	reg->setHI(UWord(temp>>32));
	std::cerr<<"multiplyunsigned\t| 0x"<<std::hex<<reg->getHI()<<" : "<<reg->getLO()<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::divu(Regidx s, Regidx t){
	//no exceptions here checked with PDF
	reg->setLO(UWord(reg->get(s))/UWord(reg->get(t)));
	reg->setHI(UWord(reg->get(s))%UWord(reg->get(t)));
	std::cerr<<"divideunsigned\t| getting LO value and HI"<<std::dec<<reg->getLO()<<"   "<<reg->getHI()<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::div(Regidx s, Regidx t){
	//no exceptions here checked with PDF
	reg->setLO(Word(reg->get(s))/Word(reg->get(t)));
	reg->setHI(Word(reg->get(s))%Word(reg->get(t)));
	std::cerr<<"div\t| getting LO value and HI"<<std::dec<<reg->getLO()<<"   "<<reg->getHI()<<" is result at PC 0x"<<std::hex<<PC<<"\n";

}

// shift more /equal to the length of the type
//http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3485.pdf, CH 5.8
void Simulator::sll(unsigned char shift,Regidx t, Regidx d){
	UWord result;
	if(shift<32){
		 result=( (reg->get(t))<<shift);
	}else{
		result=0;
	}
	reg->set(d,result);
	std::cerr<<"shiftLL\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::sllv(Regidx d , Regidx s, Regidx t){
	int shift=reg->get(s) & 0x1F;
	UWord result;
	if(shift<32){
		 result=( (reg->get(t))<<shift);
	}else{
		result=0;
	}
	reg->set(d,result);
	std::cerr<<"shiftLLVar\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::slt(Regidx d , Regidx s, Regidx t){
	if(((Word)reg->get(s))<((Word)reg->get(t))){
		reg->set(d,1);
	}
	else {
		reg->set(d,0);
	}
	//TODO might have exception
	std::cerr<<"setlt\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::sltu(Regidx d , Regidx s, Regidx t){
	if(((UWord)reg->get(s))<((UWord)reg->get(t))){
		reg->set(d,1);
	}
	else {
		reg->set(d,0);
	}
	//TODO might have exception
	std::cerr<<"setltu\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::sra(unsigned char shift,Regidx d,Regidx t){
	Word result;
	//if is required here due to shift with E2 greater than type length is undefined
	if(shift<32){
		result=( Word(reg->get(t))>>shift );
	}else{
		// such if MSB of reg[t] is 0, will result 0x0 ; is 1, will get 0xFFFFFFFF
		result=( Word(reg->get(t))>>31 );
	}
	reg->set(d,result);
	std::cerr<<"srarithmitic\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::srav(Regidx d,Regidx s,Regidx t){
	int shift=reg->get(s) & 0x1F;
	Word result;
	//if is required here due to shift with E2 greater than type length is undefined
	if(shift<32){
		result=( Word(reg->get(t))>>shift );
	}else{
		// such if MSB of reg[t] is 0, will result 0x0 ; is 1, will get 0xFFFFFFFF
		result=( Word(reg->get(t))>>31 );
	}
	reg->set(d,result);

	std::cerr<<"srarithmiticVar\t| "<<std::hex<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::srl(unsigned char shift,Regidx d,Regidx t){
	Word result;
	if(shift<32){
		result=( UWord(reg->get(t))>>shift );
	}else{
		result=0;
	}

	reg->set(d,result);
	std::cerr<<"shiftRL\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::srlv(Regidx d,Regidx t,Regidx s){
	int shift=reg->get(s) & 0x1F;
	Word result;
	if(shift<32){
		result=( UWord(reg->get(t))>>shift );
	}else{
		result=0;
	}
	reg->set(d,result);
	std::cerr<<"shiftRVar\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

//---------------------------------**********  I Type functions  **********------------------------------



void Simulator::addi(Regidx t,Regidx s, Word immediate){
	immediate=hp::sgnExtend16(immediate);
	Word temp=Word(reg->get(s))+Word(immediate);
	checkSignedOverflow(reg->get(s),immediate,temp);
	reg->set(t,temp);
	std::cerr<<"addImm\t| "<<std::dec<<reg->get(t)<<" is result at PC "<<std::hex<<PC<<"\n";
}

void Simulator::addiu(Regidx t,Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);
	UWord temp = (UWord)(reg->get(s)) + (UWord)immediate;
	// no overflow by reference
	reg->set(t,temp);
	std::cerr<<"addImmUnsigned\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::ANDI(Regidx t,Regidx s,UWord immediate){
	//already 0 extend
	UWord temp = reg->get(s) & immediate;
	reg->set(t,temp);
	std::cerr<<"ANI\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::XORI(Regidx t,Regidx s,UWord immediate){
	Word temp = (reg->get(s)) ^ immediate;
	reg->set(t,temp);
	std::cerr<<"XORI\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::ORI(Regidx t,Regidx s,UWord immediate){
	int temp = reg->get(s) | immediate;
	reg->set(t,temp);
	std::cerr<<"ORI\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::beq(Regidx t,Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);

	advPC();
	executeInstruction();

	if (reg->get(t)==reg->get(s)){
		advPCbool=false; //assert flag to disable PC+4 in run()
		PC+=immediate<<2;
	}
	else{
		//do nothing
	}
}

void Simulator::bgez(Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);

	advPC();
	executeInstruction();

	if ((Word)reg->get(s)>=0){
		advPCbool=false; //assert flag to disable PC+4 in run()
		PC+=immediate<<2;
	}
	else{
		//do nothing
	}
}

void Simulator::bgezal(Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);
	reg->set(31,PC+8);

	advPC();
	executeInstruction();

	if ((Word)reg->get(s)>=0){
		advPCbool=false;
		PC+=immediate<<2;
	}
	else{
	// do nothing
	}
	std::cerr<<"bgezal\t| "<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::bltz(Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);

	advPC();
	executeInstruction();

	if ((Word)reg->get(s)<0){
		advPCbool=false;
		PC+=immediate<<2;
	}
	else{
		//do nothing
	}
}

void Simulator::bltzal(Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);
	reg->set(31,PC+8);

	advPC();
	executeInstruction();

	if ((Word)reg->get(s)<0){
		advPCbool=false;
		PC+=immediate<<2;
	}
	else{
		//do nothing
	}
}

void Simulator::bgtz(Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);

	advPC();
	executeInstruction();

	if ((Word)reg->get(s)>0){
		advPCbool=false;
		PC+=immediate<<2;
	}
	else{
		//do nothing
	}
}

void Simulator::blez(Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);

	advPC();
	executeInstruction();

	if ((Word)reg->get(s)<=0){
		advPCbool=false;
		PC+=immediate<<2;
	}
	else{
		//do nothing
	}
}
void Simulator::bne(Regidx t,Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);

	advPC();
	executeInstruction();

	if (reg->get(t)!=reg->get(s)){
		advPCbool=false;
		PC+=immediate<<2;
	}
	else{
		//do nothing
	}
}


void Simulator::lb(Regidx t, Regidx s,Word offset){
	offset=hp::sgnExtend16(offset);
	Word byteAddr = Word(reg->get(s))+Word(offset);
	Word temp=mem->readByte(byteAddr); //memory->readbyte can handle the correct address
	temp=hp::sgnExtend8(temp);
	reg->set(t,temp);
	std::cerr<<"loadbyte\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::lbu(Regidx t, Regidx s,Word offset){
	offset=hp::sgnExtend16(offset);
	Word byteAddr = Word(reg->get(s))+Word(offset);
	Word temp=mem->readWord(byteAddr);
	reg->set(t,temp);
	std::cerr<<"loadbyte\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::sb(Regidx t, Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);
	//signed extend it otherwise no change
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	mem->writeByte(byteAddr,reg->get(t));
	std::cerr<<"storebyte\t| storing "<<reg->get(t)<<"at PC 0x"<<std::hex<<PC<<"\n";
}


void Simulator::lw(Regidx t, Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	Word temp=mem->readWord(byteAddr);
	reg->set(t,temp);
	std::cerr<<"loadword\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::lh(Regidx t, Regidx s,Word offset){
	offset=hp::sgnExtend16(offset);
	Word byteAddr = Word(reg->get(s))+Word(offset);
	Word temp = mem->readWord(byteAddr&0xfffffffd);
	Word result;
	if (byteAddr&0x2){
		//LOW half word
		result = temp &0xFFFF;
	}
	else {
		//HIGH half word
		result = temp>>16;
	}
	result=hp::sgnExtend16(result);
	reg->set(t,result);
}

void Simulator::lhu(Regidx t, Regidx s,Word offset){
	offset=hp::sgnExtend16(offset);
	Word byteAddr = Word(reg->get(s))+Word(offset);
	Word temp = mem->readWord(byteAddr&0xfffffffd);
	Word result;
	if (byteAddr&0x2){
		result = temp &0xFFFF;
	}else {
		result = temp>>16;
	}
	reg->set(t,result);

}

void Simulator::sw(Regidx t, Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	mem->writeWord(byteAddr,reg->get(t));
	std::cerr<<"storeword\t| "<<std::hex<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::sh(Regidx t, Regidx s,Word immediate){
	immediate=hp::sgnExtend16(immediate);
	Word HWAddr = Word(reg->get(s))+Word(immediate);
	mem->writeHalfword(HWAddr,reg->get(t)&0xFFFF);
	std::cerr<<"storehalfword\t| "<<std::hex<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::lwl(Regidx t, Regidx s, Word immediate){
	//TODO not reviewed: write testcase
	immediate=hp::sgnExtend16(immediate);
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	int index=byteAddr%4;

	//first part of OR => the required word from memory (shift word) | 2nd part => the register to keep (shift mask)
	UWord result=( UWord(mem->readWord(byteAddr&0xfffffffc)) << 8*index) | (reg->get(t) & (UWord(0x00FFFFFF)>>8*(3-index)));

	reg->set(t,result);
	std::cerr<<"loadwordleft\t| "<<std::hex<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::lwr(Regidx t, Regidx s, Word immediate){
	//add case !
	immediate=hp::sgnExtend16(immediate);
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	int index=byteAddr%4;
	//first part of OR => the required word from memory (shift word) | 2nd part => the register to keep (shift mask)
	UWord result=( UWord(mem->readWord(byteAddr&0xfffffffc)) >> 8*(3-index) ) | (reg->get(t) & (UWord(0xFFFFFF00)<<8*index));

	reg->set(t,result);
	std::cerr<<"loadwordright\t| "<<std::hex<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::lui(Regidx t,UWord immediate){
	Word temp=immediate<<16;
	reg->set(t,temp);
	std::cerr<<"loadupperImm\t| "<<std::hex<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::slti(Regidx t, Regidx s, Word immediate){
	immediate=hp::sgnExtend16(immediate);

	if (Word(reg->get(s))<Word(immediate)){
		reg->set(t,1);
	}
	else {
		reg->set(t,0);
	}
	std::cerr<<"setlessthan_Imm_signed\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::sltiu(Regidx t, Regidx s, UWord immediate){
	if (UWord(reg->get(s))<UWord(immediate)){
		reg->set(t,1);
	}
	else {
		reg->set(t,0);
	}
	std::cerr<<"setlessthan_Imm_Usigned\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}
//*********************************   Jump instructions *******************************************************

void Simulator::j(Word instr_index){

	advPC();
	executeInstruction();
	advPCbool=false;
	PC=(instr_index<<2)| (PC&0xF0000000); //remaining upper bits are the corresponding bits of the address of the instruction in the delay slot
}

void Simulator::jal(Word instr_index){

	advPC();
	reg->set(31,PC+4);
	executeInstruction();
	advPCbool=false;
	PC=(instr_index<<2)| (PC&0xF0000000);
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
