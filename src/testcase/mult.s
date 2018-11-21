#mult,25,,,$LO should return 25
addiu $3,$0,5
mult $3,$3
mfhi $14
mflo $2
jr $0
