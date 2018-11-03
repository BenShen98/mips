#divu,81,,,R 735 Q 81
addiu $1,$0,65535
addiu $3,$0,800
divu $1,$3
mfhi $1
mflo $2
jr $0
