typedef unsigned char Regidx;
typedef unsigned int Word;
#include "Memory.hpp"
#include "Register.hpp"
class Simulator{
	public:

		void run();
		Simulator();
		~Simulator();

	private:
		Register * reg;//HI/LO only access by mfhi/mfhlo
		//TODO register zero = 0 !
		Memory * mem;
		int PC=0x10000000;

		void Rswitch();
		//Exception functions here
		void ISAexception();
		void Mathexception();/* code */
		void Memexception();

		//All functions here
		void add(Regidx d,Regidx s,Regidx t);
		void addu(Regidx d,Regidx s,Regidx t);
		void jr(Regidx s);
		void andbitwise(Regidx d,Regidx s,Regidx t);
		void orbitwise(Regidx d,Regidx s,Regidx t);
		void xorbitwise(Regidx d,Regidx s,Regidx t);
		void mfhi(Regidx d );
		void mflo(Regidx d );
		void multiply(Regidx s,Regidx t);
		void multiplyu(Regidx s,Regidx t);
};
