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

		//All functions here
		void add(Regidx d,Regidx s,Regidx t);
		void addu(Regidx d,Regidx s,Regidx t);
		void sub(Regidx d,Regidx s,Regidx t);
		void subu(Regidx d,Regidx s,Regidx t);
		void mult(Regidx s,Regidx t);
		void multu(Regidx s,Regidx t);
		void div(Regidx s,Regidx t);
		void divu(Regidx s,Regidx t);
		void jr(Regidx s);
		void andbitwise(Regidx d,Regidx s,Regidx t);
		void orbitwise(Regidx d,Regidx s,Regidx t);
		void xorbitwise(Regidx d,Regidx s,Regidx t);
		void mfhi(Regidx d );
		void mflo(Regidx d );
		void mtlo(Regidx d);
		void mthi(Regidx d);
		void sll(unsigned char shift,Regidx t, Regidx d);
		void sllv(Regidx d , Regidx s, Regidx t);
		void slt(Regidx d , Regidx s, Regidx t);
		void sltu(Regidx d , Regidx s, Regidx t);
		void sra(unsigned char shift,Regidx d,Regidx t);
		void srav(Regidx d, Regidx s,Regidx t);
		void srl(unsigned char shift,Regidx d,Regidx t);
		void srlv(Regidx d,Regidx t,Regidx s);
		//Below are immediate instructions
		void addi(Regidx t,Regidx s, Word immediate);
		void addiu(Regidx t,Regidx s,Word immediate);
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

		inline void checkSignedOverflow(Word a, Word b, Word result);

		//below are Jumo instruction
		void j(Word instr_index);
		void jal(Word instr_index);
		void jalr(Regidx d,Regidx s);
		//Below are memory instructions
		void lb(Regidx t, Regidx s,Word offset);
		void lbu(Regidx t, Regidx s,Word offset);
		void lh(Regidx t, Regidx s,Word offset);
		void lhu(Regidx t, Regidx s,Word offset);
		void lui(Regidx t,UWord immediate);
		void lw(Regidx t, Regidx s, Word addr);
		void lwl(Regidx s, Regidx t, Word immediate);
		void lwr(Regidx s, Regidx t, Word immediate);
		void sb(Regidx t, Regidx s, Word immediate);
		void sh(Regidx t, Regidx s,Word immediate);
		void sw(Regidx t, Regidx s, Word immediate);
		void slti(Regidx t, Regidx s, Word immediate);
		void sltiu(Regidx t, Regidx s, UWord immediate);

		inline signed char word2Sbyte(Word word, int idx);



	};
