typedef unsigned int Word;
typedef unsigned char Regidx;

class Register{
public:
	Word get(Regidx i);
	Word getHI();
	Word getLO();
	void set(Regidx i, Word value);

	void RegException();

private:
	Word reg[34]={0,1,2,3,0};
};
