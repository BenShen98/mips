#bltz,0,,,testing bltz
addi $1,$0,65534
addiu $3,$0,1
loop: addu  $1,$1,$3
bltz  $1,loop
addu  $2,$0,$1
jr $0
