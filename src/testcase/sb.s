#sb,101,64,65,testing all loadb and storeb
lui $14,12288
lw  $2,0($14)
#addi $2,$0,0x64
addiu $2,$2,1
addiu $14,$14,4
sb  $2,3($14)
jr  $0
