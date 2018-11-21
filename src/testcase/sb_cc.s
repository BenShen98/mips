#sb_cc,245,,,store byte to invaild addr -11
lui $14,4096 #0x10000000
addiu $2,$0,16
sb   $2,2($14)
jr   $0
