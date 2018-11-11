#bgez,156,,,testing bgez with negative output
addiu $1,$0,300
addiu $3,$0,100
loop: sub $1,$1,$3
bgez  $1,loop
jr $0
