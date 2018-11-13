#bgezal,200,,,testing bgez should return 200
addiu $2,$0,300
addiu $3,$0,100
bgezal  $2,end
sub $2,$2,$3
sub $2,$2,$3
jr $0
end: addi $2,$2,100
jr $31
