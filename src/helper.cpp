//NOTE: DOES THIS FILE NEED COMPILE DIRECTLY???

namespace hp{
  inline Word sgnExtend16(Word input){
  	if (input & 0x8000){
  		input = (input | (0xFFFF0000));
  	}
  	return input;
  }

  inline Word sgnExtend8(Word input){
  	if (input&0x80){
  		input=input|0xFFFFFF00;
  	}
  	return input;
  }
}
