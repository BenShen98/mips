#bgtz,158,,,testing bgtz
addiu $1,$0,65534
addiu $3,$0,6000
loop: subu  $1,$1,$3
bgtz  $1,loop
add $2,$1,$3
jr $0
