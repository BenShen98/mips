#bitwise,255,,,testing all bit wise logic
addiu $1,$0,65535
addiu $3,$0,65535
and  $2,$1,$3
or   $4,$1,$3
xor  $5,$1,$3
jr $0
