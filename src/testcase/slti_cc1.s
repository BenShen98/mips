#slti,1,,,testing immediate sign extend
li $3,0xFFFFFFFE #-2
slti $2,$3,0xFFFF
jr $0
