typedef unsigned int Word;

class Register{
public:
	Word get(Regidx i);
	Word getHI();
	Word getLO();
	void set(Regidx i, Word value);

private:
	Word reg[34]={0};
}
