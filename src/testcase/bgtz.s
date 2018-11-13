#bgtz,100,,,testing bgtz with positive output
addiu $14,$0,300
addiu $3,$0,100
loop: sub  $14,$14,$3
bgtz  $14,loop
add $2,$14,$3
jr $0
