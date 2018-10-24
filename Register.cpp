Word Register::get(Regidx i){
	if(i>31){
		std::cerr << "reading invaild register address"<<std::hex<<PC << '\n';
		Memexception();
	}

	if(i==0){
		return Word(0);//type cast
	}
	return reg[i];
}

Word Register::getHI(){
	return reg[33];
}

Word Register::getLO(){
	return reg[32];
}

void Register::set(Regidx i, Word value){

	//error check
	if(i==0){
		std::cerr << "WARNING: writting to register 0 at PC "<<std::hex<<PC << '\n';
	}

	if(i>31){
		std::cerr << "reading invaild register address"<<std::hex<<PC << '\n';
		Memexception();
	}

	reg[i]=value;
}
