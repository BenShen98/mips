addi $2,$0,5
test:addi $1,$1,1
beq $2,$1, end
j test
end: JR $0
