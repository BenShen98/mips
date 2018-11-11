#lwl,0,,,using instrcution mem for testing
addi $1,$2,5
addi $3,$4,6
addi $5,$6,8
lui $7,4096
lwl  $2,2($7)
jr $0
