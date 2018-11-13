#multu,1,,,comment here
addiu $14,$0,65535
sll  $3,$14,15
ori $3,$3,65535
multu $3,$3
mfhi $14
mflo $2
jr $0
