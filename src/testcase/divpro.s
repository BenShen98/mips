#divpro,1,,,R 535 Q 1
addi $1,$0,65535
addi $3,$0,65000
divu $1,$3
mfhi $1
mflo $2
jr $0
