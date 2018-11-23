#divu,10,0,,test divu by 0

li $3,0x30000000
lw $4,0($3)
divu $3,$4 #divu R3 by 0
xor $2,$0,10
jr $0
