#beq,4,,,testing beq should return 4
addiu $14,$0,6
addiu $2,$0,2
loop: subu  $14,$14,$2
beq  $14,$0,loop
addu  $2,$0,$14
jr $0
