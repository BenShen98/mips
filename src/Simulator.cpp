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
		addImm(t,s,immediate);break;
		case 0b001001:
		addImmUnsigned(t,s,immediate);break;
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
		loadbyte(t,s,immediate);break;
		case 0b100100:
		loadUbyte(t,s,immediate);break;
		case 0b001111:
		loadupperImm(t,immediate);break;
		case 0b100011:
		loadword(t,s,immediate);break;
		case 0b100001:
		loadhalfword(t,s,immediate);break;
		case 0b100101:
		loadhalfwordU(t,s,immediate);break;
		case 0b100010:
		loadwordleft(t,s,immediate);break;
		case 0b100110:
		loadwordright(t,s,immediate);break;
		case 0b101000:
		storebyte(t,s,immediate);break;
		case 0b101011:
		storeword(t,s,immediate);break;
		case 0b101001:
		storehalfword(t,s,immediate);break;
		case 0b001010:
		setlessthan_Imm_signed(t,s,immediate);break;
		case 0b001011:
		setlessthan_Imm_Usigned(t,s,immediate);break;
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
		case 0b011000: multiply(s,t);break;
		case 0b011001: multiplyunsigned(s,t);break;
		case 0b001000: jr(s);break;
		case 0b100100: andbitwise(d,s,t);break;
		case 0b100101: orbitwise(d,s,t);break;
		case 0b100110: xorbitwise(d,s,t);break;
		case 0b010000: mfhi(d);break;
		case 0b010001: mthi(d);break;
		case 0b010010: mflo(d);break;
		case 0b010011: mtlo(d);break;
		case 0b000000: LLshift(shift,t,d);break;
		case 0b000100: shiftLLVar(d,s,t);break;
		case 0b101010: setlt(d,s,t);break;
		case 0b101011: setltu(d,s,t);break;
		case 0b000011: shiftRA(shift,d,t);break;
		case 0b000111: shiftRAVar(d,s,t);break;
		case 0b000010: shiftRL(shift,d,t);break;
		case 0b000110: shiftRVar(d,t,s);break;
		case 0b011010: dividesigned(s,t);break;
		case 0b011011: divideunsigned(s,t);break;
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
	int temp = reg->get(s) ^ reg->get(t);
	reg->set(d,temp);
	std::cerr<<"xor\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::mthi(Regidx d){
	reg->setHI(reg->get(d));
}

void Simulator::mtlo(Regidx d){
	reg->setLO(reg->get(d));
}

void Simulator::mfhi(Regidx d ){
	reg->set(d,reg->getHI());
	std::cerr<<"mfhi\t| "<<std::dec<<reg->getHI()<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::mflo(Regidx d ){
	reg->set(d,reg->getLO());
	std::cerr<<"mflo\t| "<<std::dec<<reg->getLO()<<" is result at PC 0x"<<std::hex<<PC<<"\n";
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
	reg->setLO(UWord(reg->get(s))/UWord(reg->get(t)));
	reg->setHI(UWord(reg->get(s))%UWord(reg->get(t)));
	std::cerr<<"divideunsigned\t| getting LO value and HI"<<std::dec<<reg->getLO()<<"   "<<reg->getHI()<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::dividesigned(Regidx s, Regidx t){
	//no exceptions here checked with PDF
	reg->setLO(Word(reg->get(s))/Word(reg->get(t)));
	reg->setHI(Word(reg->get(s))%Word(reg->get(t)));
	std::cerr<<"div\t| getting LO value and HI"<<std::dec<<reg->getLO()<<"   "<<reg->getHI()<<" is result at PC 0x"<<std::hex<<PC<<"\n";

}

void Simulator::LLshift(unsigned char shift,Regidx t, Regidx d){
	long temp = (long(reg->get(t))<<shift);
	//no exception detection for unsigned op
	reg->set(d,(int)temp);
	std::cerr<<"shiftLL\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::shiftLLVar(Regidx d , Regidx s, Regidx t){
	reg->set(d,(reg->get(t))<<(reg->get(s)));
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
	if(((unsigned int)reg->get(s))<((unsigned int)reg->get(t))){
		reg->set(d,1);
	}
	else {
		reg->set(d,0);
	}
	//TODO might have exception
	std::cerr<<"setltu\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::shiftRA(unsigned char shift,Regidx d,Regidx t){
	reg->set(d,Word(reg->get(t))>>shift);
	std::cerr<<"shiftRArithmitic\t| "<<std::dec<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::shiftRAVar(Regidx d,Regidx s,Regidx t){
	reg->set(d,Word(reg->get(t))>>(reg->get(s)));
	std::cerr<<"shiftRArithmiticVar\t| "<<std::hex<<reg->get(d)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
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

inline Word Simulator::sgnExtend16(Word input){
	if (input & 0x8000){
		input = (input | (0xFFFF0000));
	}
	return input;
}

void Simulator::addImm(Regidx t,Regidx s, Word immediate){
	immediate=sgnExtend16(immediate);
	Word temp=Word(reg->get(s))+Word(immediate);
	checkSignedOverflow(reg->get(s),immediate,temp);
	reg->set(t,temp);
	std::cerr<<"addImm\t| "<<std::dec<<reg->get(t)<<" is result at PC "<<std::hex<<PC<<"\n";
}

void Simulator::addImmUnsigned(Regidx t,Regidx s,Word immediate){
	immediate=sgnExtend16(immediate);
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
	immediate=sgnExtend16(immediate);

	advPC();
	executeInstruction();

	if (reg->get(t)==reg->get(s)){
		advPCbool=false; //assert flag to disable PC+4 in run()
		PC+=immediate<<2;
	}
	else{
		//do noting
	}
}

void Simulator::bgez(Regidx s,Word immediate){
	immediate=sgnExtend16(immediate);

	advPC();
	executeInstruction();

	if ((Word)reg->get(s)>=0){
		advPCbool=false; //assert flag to disable PC+4 in run()
		PC+=immediate<<2;
	}
	else{
		//do noting
	}
}

void Simulator::bgezal(Regidx s,Word immediate){
	immediate=sgnExtend16(immediate);
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
	immediate=sgnExtend16(immediate);

	advPC();
	executeInstruction();

	if ((Word)reg->get(s)<0){
		advPCbool=false;
		PC+=immediate<<2;
	}
	else{
		//do noting
	}
}

void Simulator::bltzal(Regidx s,Word immediate){
	immediate=sgnExtend16(immediate);
	reg->set(31,PC+8);

	advPC();
	executeInstruction();

	if ((Word)reg->get(s)<0){
		advPCbool=false;
		PC+=immediate<<2;
	}
	else{
		//do noting
	}
}

void Simulator::bgtz(Regidx s,Word immediate){
	immediate=sgnExtend16(immediate);

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
	immediate=sgnExtend16(immediate);

	advPC();
	executeInstruction();

	if ((Word)reg->get(s)<=0){
		advPCbool=false;
		PC+=immediate<<2;
	}
	else{
		//do noting
	}
}
void Simulator::bne(Regidx t,Regidx s,Word immediate){
	immediate=sgnExtend16(immediate);

	advPC();
	executeInstruction();

	if (reg->get(t)!=reg->get(s)){
		advPCbool=false;
		PC+=immediate<<2;
	}
	else{
		//do noting
	}
}

inline Word Simulator::sgnExtend8(Word input){
	if (input&0x80){
		input=input|0xFFFFFF00;
	}
	return input;
}

void Simulator::loadbyte(Regidx t, Regidx s,Word offset){
	offset=sgnExtend16(offset);
	Word byteAddr = Word(reg->get(s))+Word(offset);
	Word temp=mem->readByte(byteAddr); //memory->readbyte can handle the correct address
	temp=sgnExtend8(temp);
	reg->set(t,temp);
	std::cerr<<"loadbyte\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::loadUbyte(Regidx t, Regidx s,Word offset){
	offset=sgnExtend16(offset);
	Word byteAddr = Word(reg->get(s))+Word(offset);
	Word temp=mem->readWord(byteAddr);
	reg->set(t,temp);
	std::cerr<<"loadbyte\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::storebyte(Regidx t, Regidx s,Word immediate){
	if (immediate & 0x8000){
		immediate = (immediate | (0xFFFF0000));
	}
	//signed extend it otherwise no change
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	mem->writeByte(byteAddr,reg->get(t));
	std::cerr<<"storebyte\t| storing "<<reg->get(t)<<"at PC 0x"<<std::hex<<PC<<"\n";
}


void Simulator::loadword(Regidx t, Regidx s,Word immediate){
	immediate=sgnExtend16(immediate);
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	Word temp=mem->readWord(byteAddr);
	reg->set(t,temp);
	std::cerr<<"loadword\t| "<<std::dec<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::loadhalfword(Regidx t, Regidx s,Word offset){
	offset=sgnExtend16(offset);
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
	result=sgnExtend16(result);
	reg->set(t,result);
}

void Simulator::loadhalfwordU(Regidx t, Regidx s,Word offset){
	offset=sgnExtend16(offset);
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

void Simulator::storeword(Regidx t, Regidx s,Word immediate){
	if (immediate & 0x8000){
		immediate = (immediate | (0xFFFF0000));
	}
	//check immediate overflow
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	mem->writeWord(byteAddr,reg->get(t));
	std::cerr<<"storeword\t| "<<std::hex<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::storehalfword(Regidx t, Regidx s,Word immediate){
	if (immediate & 0x8000){
		immediate = (immediate | (0xFFFF0000));
	}
	//check immediate overflow
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	Word result=(reg->get(t)&0xFFFF) | (mem->readWord(byteAddr)&0xffff0000);
	mem->writeWord(byteAddr,result);
	std::cerr<<"storehalfword\t| "<<std::hex<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::loadwordleft(Regidx t, Regidx s, Word immediate){
	//TODO not reviewed: write testcase
	immediate=sgnExtend16(immediate);
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	int index=byteAddr%4;
	//first part of OR => the required word from memory | 2nd part => the register to keep
	UWord result=((UWord)mem->readWord(byteAddr&0xfffffffc) << 8*index) | (reg->get(t) & (UWord(0x00FFFFFF)>>8*(3-index)));
	reg->set(t,result);
	std::cerr<<"loadwordleft\t| "<<std::hex<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::loadwordright(Regidx t, Regidx s, Word immediate){
	//add case !
	immediate=sgnExtend16(immediate);
	Word byteAddr = Word(reg->get(s))+Word(immediate);
	int index=byteAddr%4;
	//first part of OR => the required word from memory | 2nd part => the register to keep
	UWord result=((UWord)mem->readWord(byteAddr&0xfffffffc) >> 8*(3-index)) | (reg->get(t) & (UWord(0xFFFFFF00)<<8*index));
	std::cerr << "/* error message */" <<result<< '\n';
	reg->set(t,result);
	std::cerr<<"loadwordleft\t| "<<std::hex<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
}

void Simulator::loadupperImm(Regidx t,UWord immediate){
	Word temp=immediate<<16;
	reg->set(t,temp);
	std::cerr<<"loadupperImm\t| "<<std::hex<<reg->get(t)<<" is result at PC 0x"<<std::hex<<PC<<"\n";
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
