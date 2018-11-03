#sw,101,64,65,testing all loadw and storew
lui $1,12288
lw  $2,0($1)
addiu $2,$2,1
addiu $1,$1,4
sw  $2,0($1)
jr  $0
