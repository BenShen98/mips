#bitwise,255,,,testing all bit wise logic
addiu $14,$0,65535
addiu $3,$0,65535
and  $2,$14,$3
or   $4,$14,$3
xor  $5,$14,$3
jr $0
