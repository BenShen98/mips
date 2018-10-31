#include "Register.hpp"
#include <iostream>



Word Register::get(Regidx i){
	if(i>31){
		RegException();
	}

	if(i==0){
		return Word(0);//type cast
	}
	return reg[i];
}

Word Register::getHI(){
	return reg[33];
}

Word Register::setHI(Word value){
	reg[33]=value;
}

Word Register::setLO(Word value){
	reg[32]=value;
}

Word Register::getLO(){
	return reg[32];
}

void Register::set(Regidx i, Word value){

	//error check
	if(i==0){
		std::cerr << "WARNING: writting to register 0 at PC 0x"<< '\n';
	}

	if(i>31){
		RegException();
	}

	reg[i]=value;
}

void Register::RegException(){
	std::cerr << "reading invaild register address"<< '\n';
	// Memexception();
	std::exit(-1008);
}
