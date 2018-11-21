#multu,36,,,should return 36
addiu $3,$0,6
multu $3,$3
mfhi $3
mflo $2
jr $0
