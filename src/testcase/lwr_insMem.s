#lwl,65,,,using instrcution mem for testing
addi $1,$2,5
addi $3,$4,6
addi $5,$6,8
lui $7,4096
lwr  $2,1($7)
jr $0
