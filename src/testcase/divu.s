#divu,6,,,R 200 Q 6
addiu $1,$0,5000
addiu $3,$0,800
divu $1,$3
mfhi $1
mflo $2
jr $0
