#lwl_cc,245,,59,address error 245
lui  $14,12288 #0x30000000 read only
addiu $2,$2,5465
sw   $2,4($14) #store 0x1559 to 0x30000004
lwl   $2,5($14)
jr   $0
