#include <fstream>      // std::ifstream, std::ofstream

int main () {
  std::ofstream outfile("ins.bin",std::ofstream::binary);

  const int insLength=4;

  unsigned int memInstruction[insLength]={
    0b00100000001000000001101100100000,
    0b00100000010000000011111110000011,
    0b00000000001000100001100000100001,
    0b00000000000000000000000000001000
  };

  char* buffer = new char[insLength*4];

  long buffPt=0;
  for (int i=0;i<insLength;i++){
    unsigned int inst=memInstruction[i];
    buffer[buffPt+3]= (inst&0xff);//LSB
    buffer[buffPt+2]= (inst&0xff00)>>8;
    buffer[buffPt+1]= (inst&0xff0000)>>16;
    buffer[buffPt]= (inst&0xff000000)>>24;//MSB
    buffPt=buffPt+4;
  }

  // write to outfile
  outfile.write(buffer,insLength*4);


  // release dynamically-allocated memory
  delete[] buffer;

  outfile.close();
  return 0;
}
