#bgez,0,,,testing bgez
addiu $1,$0,60000
addiu $3,$0,6000
loop: subu  $1,$1,$3
bgez  $1,loop
add $2,$1,$3
jr $0
