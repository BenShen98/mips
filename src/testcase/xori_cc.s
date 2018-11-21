#xori_cc,2,,,xori should not sign extend
addiu $4,$0,0x7FFF
xori  $2,$4,0xFFFF
srl $2,$2,14
jr $0
