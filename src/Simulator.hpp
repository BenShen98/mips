typedef unsigned int Regidx;
typedef signed int Word;
typedef unsigned int UWord;
#include "Memory.hpp"
#include "Register.hpp"
class Simulator{
	public:

		void run();
		Simulator(char* instructionFile);
 		~Simulator();

	private:
		Register * reg;//HI/LO only access by mfhi/mfhlo
		//TODO register zero = 0 !
		Memory * mem;
		int PC=0x10000000;
    bool advPCbool=true;

    inline void executeInstruction();
    inline void advPC();

		void Rswitch();
		void BranchSwitch();
		//Exception functions here
		void ISAexception();
		void Mathexception();/* code */
		void Memexception();

		//All functions here
		void add(Regidx d,Regidx s,Regidx t);
		void addu(Regidx d,Regidx s,Regidx t);
		void sub(Regidx d,Regidx s,Regidx t);
		void subu(Regidx d,Regidx s,Regidx t);
		void multiply(Regidx s,Regidx t);
		void multiplyunsigned(Regidx s,Regidx t);
		void dividesigned(Regidx s,Regidx t);
		void divideunsigned(Regidx s,Regidx t);
		void jr(Regidx s);
		void andbitwise(Regidx d,Regidx s,Regidx t);
		void orbitwise(Regidx d,Regidx s,Regidx t);
		void xorbitwise(Regidx d,Regidx s,Regidx t);
		void mfhi(Regidx d );
		void mflo(Regidx d );
		void mtlo(Regidx d);
		void mthi(Regidx d);
		void LLshift(unsigned char shift,Regidx t, Regidx d);
		void shiftLLVar(Regidx d , Regidx s, Regidx t);
		void setlt(Regidx d , Regidx s, Regidx t);
		void setltu(Regidx d , Regidx s, Regidx t);
		void shiftRA(unsigned char shift,Regidx d,Regidx t);
		void shiftRL(unsigned char shift,Regidx d,Regidx t);
		void shiftRVar(Regidx d,Regidx t,Regidx s);
		//Below are immediate instructions
		void addImm(Regidx t,Regidx s, Word immediate);
		void addImmUnsigned(Regidx t,Regidx s,UWord immediate);
		void ANDI(Regidx t,Regidx s,UWord immediate);
		void XORI(Regidx t,Regidx s,UWord immediate);
		void ORI(Regidx t,Regidx s,UWord immediate);
		//Below are branch instructions
		void beq(Regidx t,Regidx s,Word immediate);
		void bgez(Regidx s,Word immediate);
		void bgezal(Regidx s,Word immediate);
		void bltz(Regidx s,Word immediate);
		void bltzal(Regidx s,Word immediate);
		void bgtz(Regidx s,Word immediate);
		void blez(Regidx s,Word immediate);
		void bne(Regidx t,Regidx s,Word immediate);

		//below are Jumo instruction
		void j();
		void jal();
		void jalr(Regidx d,Regidx s);
		//Below are memory instructions
		void loadbyte(Regidx t, Regidx s,Word immediate);
		void loadUbyte(Regidx t, Regidx s,Word immediate);
		void loadhalfword(Regidx t, Regidx s,Word immediate);
		void loadhalfwordU(Regidx t, Regidx s,Word immediate);
		void loadupperImm(Regidx t,UWord immediate);
		void loadword(Regidx t, Regidx s, Word addr);
		void loadwordleft(Regidx s, Regidx t, Word immediate);
		void loadwordright(Regidx s, Regidx t, Word immediate);
		void storebyte(Regidx t, Regidx s, Word immediate);
		void storehalfword();
		void storeword(Regidx t, Regidx s, Word immediate);
		void setlessthan_Imm_signed(Regidx t, Regidx s, Word immediate);
		void setlessthan_Imm_Usigned(Regidx t, Regidx s, UWord immediate);

		inline signed char word2Sbyte(Word word, int idx);



	};
