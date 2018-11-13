#bltz,1,,,testing bltz should return 1
addi $2,$0,65533
# $2 is -3
loop: addi $2,$2,1
blez  $2,loop
jr $0
