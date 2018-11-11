#bgez,156,,,testing bgez with negative output
addiu $2,$0,300
addiu $3,$0,100
loop: sub $2,$2,$3
bgez  $2,loop
jr $0
