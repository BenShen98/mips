#bgtz,100,,,testing bgtz with positive output
addiu $1,$0,300
addiu $3,$0,100
loop: sub  $1,$1,$3
bgtz  $1,loop
add $2,$1,$3
jr $0
