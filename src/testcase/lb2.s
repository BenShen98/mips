#lb2,1,,,should return 1
lui  $14,8192
addiu $2,$2,255
sb   $2,2($14)
lb   $2,2($14)
srl  $2,$2,31
jr   $0