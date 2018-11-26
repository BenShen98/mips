#j,20,,,here is for comment
addi $2,$0,1
addi $4,$0,20
loop:addi $2,$2,1
beq $4,$2,end
j loop
end: jr $0
