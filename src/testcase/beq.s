#beq,4,,,testing beq should return 4
addiu $1,$0,6
addiu $2,$0,2
loop: subu  $1,$1,$2
beq  $1,$0,loop
addu  $2,$0,$1
jr $0
