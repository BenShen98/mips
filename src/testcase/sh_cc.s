#sh,245,,,mem exception 245
lui $14,8192 #0x20000000
addiu $2,$0,8
sh  $2,1($14)
lw  $2,0($14)
srl $2,$2,16
jr  $0
