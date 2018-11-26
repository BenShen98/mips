#bgez,253,,,comparing negative values should return -3
addiu $2,$0,0xFFFE #-2
addi  $4,$0,1
loop: sub $2,$2,$4
bgez  $2,loop
jr $0
