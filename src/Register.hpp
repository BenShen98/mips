typedef unsigned int Regidx;
typedef signed int Word;
typedef unsigned int UWord;

class Register{
public:
	Word get(Regidx i);
	Word getHI();
	Word getLO();
	Word setHI(Word value);
	Word setLO(Word value);
	void set(Regidx i, Word value);

	void RegException();

private:
	Word reg[34]={0};
};