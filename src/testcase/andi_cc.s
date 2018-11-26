#andi,3,,,andi should not sign extend
addiu $4,$0,0xFFFF
andi  $2,$4,0xFFFF
srl $2,$2,14
jr $0
