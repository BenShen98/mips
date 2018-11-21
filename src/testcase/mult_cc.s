#mult_cc,1,,,$LO should return 1
lui $3,65535
ori $3,$3,65535 #-1*-1
mult $3,$3
mfhi $14
mflo $2
jr $0
