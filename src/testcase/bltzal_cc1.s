#bltzal_cc1,21,,,testing $31 next+4 and add result
addiu $2,$0,0xFFFE #-2
addiu $3,$0,1
bltzal  $2,end
add $2,$2,$3
add $2,$2,$31
jr $0
end: addi $2,$2,10
jr $31
