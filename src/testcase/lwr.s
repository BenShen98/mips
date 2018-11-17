#lwr,21,,,should return 21
lui  $14,8192 #0x20000000
addiu $2,$2,5465
sw   $2,0($14) #store 0x1559
lwr   $2,2($14)
jr   $0
