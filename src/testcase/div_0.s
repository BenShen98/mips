#div,10,0,,test div by 0

li $3,0x30000000
lw $4,0($3)
div $3,$4 #div R3 by 0
xor $2,$0,10
jr $0
