#lhu,245,,,should return -11(245)
lui  $14,8192
addiu $2,$2,5465
sw   $2,2($14)
lhu   $2,2($14)
jr   $0
