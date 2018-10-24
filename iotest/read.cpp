#include <iostream>
#include <fstream>
#include <bitset>
using namespace std;

int main () {
  streampos size;
  char * memblock;

  ifstream file ("ins.bin", ios::in|ios::binary|ios::ate);
  if (file.is_open())
  {
    size = file.tellg();
    memblock = new char [size];
    file.seekg (0, ios::beg);
    file.read (memblock, size);
    file.close();

    cout << "the entire file content is in memory\n";
    unsigned int* memInstruction = new unsigned int[0x10000];

    for (int i=0; i<size;i++){
      std::cerr << std::bitset<8>(memblock[i]) << '\n';
    }

    delete[] memblock;
  }
  else cout << "Unable to open file";
  return 0;
}
