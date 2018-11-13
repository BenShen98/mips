#sh,8,,,should return 8
lui $14,8192
addiu $2,$0,8
sh  $2,0($14)
lw  $2,0($14)
srl $2,$2,16
jr  $0
