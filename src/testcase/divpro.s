#divpro,1,,,R 535 Q 1
addi $14,$0,65535
addi $3,$0,65000
divu $14,$3
mfhi $14
mflo $2
jr $0
