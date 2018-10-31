addiu $1,$0,65535
sll  $3,$1,15
ori $3,$3,65535
multu $3,$3
mfhi $1
mflo $2
jr $0
