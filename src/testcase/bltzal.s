#bltzal,99,,,testing bgez should return 99
addiu $2,$0,65533
addiu $3,$0,1
bltzal  $2,end
add $2,$2,$3
add $2,$2,$3
jr $0
end: addi $2,$2,100
jr $31
