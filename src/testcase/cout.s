addi $2,$0,0x43

#mem[0x30000004] << $2
addi $3,$0,3
sll $3,$3,28
sw $2,4($3)
addi $2,$0,0x1
sw $2,4($3)


addi $2,$0,0
addi $2,$0,0
JR $0
