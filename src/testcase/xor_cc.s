#xor_cc,255,,,testing XOR
addiu $4,$0,0xFFFF
addiu $3,$0,0x8000
xor  $2,$4,$3
jr $0
