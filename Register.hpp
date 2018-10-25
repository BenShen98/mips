typedef unsigned char Regidx;
typedef signed int Word;
typedef unsigned int UWord;

class Register{
public:
	Word get(Regidx i);
	Word getHI();
	Word getLO();
	void set(Regidx i, Word value);

	void RegException();

private:
	Word reg[34]={0};
};
