#lbu,255,,,should return 255
lui  $14,8192 #0x20000000
addiu $2,$2,65535
sb   $2,2($14)
lbu   $2,2($14)
jr   $0
