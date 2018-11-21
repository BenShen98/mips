#sllv,20,,,logical left shift should return 20
addiu $3,$0,5
addi $2,$0,2
sll $2,$3,$2
jr $0
