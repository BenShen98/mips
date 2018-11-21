#divu_cc,81,,,Q  81
addiu $14,$0,65535
addiu $3,$0,800
divu $14,$3
mfhi $14
mflo $2
jr $0
