#bgezal,12,,,test if $31 is next+4
addiu $2,$0,300
addiu $3,$0,100
bgezal  $2,end
sub $2,$2,$3
sub $2,$2,$3
addu $2,$0,$31
jr $0
end: addi $2,$2,100
jr $31
