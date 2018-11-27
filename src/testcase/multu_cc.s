#multu,254,,,should return 254 with no errors
lui $3,65535
ori $3,$3,65535
multu $3,$3
mfhi $2
mflo $14
jr $0
