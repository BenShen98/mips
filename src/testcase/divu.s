#divu,6,,,R 200 Q 6
addiu $14,$0,5000
addiu $3,$0,800
divu $14,$3
mfhi $14
mflo $2
jr $0
