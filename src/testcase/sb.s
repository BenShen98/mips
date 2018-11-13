#sb,101,64,65,testing all loadb and storeb
lui $1,12288
lw  $2,0($1)
#addi $2,$0,0x64
addiu $2,$2,1
addiu $1,$1,4
sb  $2,3($1)
jr  $0
