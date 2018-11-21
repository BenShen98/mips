#div_cc,0,,,dividing 0
addi $14,$0,65535
addi $3,$0,0
div $14,$3
mfhi $14
mflo $2
jr $0
