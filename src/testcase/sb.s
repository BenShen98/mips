#sb,16,,,store byte
lui $14,8192 #0x20000000
addiu $2,$0,16
sb   $2,2($14)
jr   $0
