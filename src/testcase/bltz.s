#bltz,0,,,testing bltz should return 0
addi $2,$0,65533
# $2 is -3
loop: addi $2,$2,1
bltz  $2,loop
jr $0
