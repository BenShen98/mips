#lhu,89,,,should return 89
lui  $14,8192
addiu $2,$2,5465
sw   $2,2($14)
lhu   $2,2($14)
jr   $0
