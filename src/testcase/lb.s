#lb,16,,,testing all loadb and storeb
addiu $1,$0,8192
addiu $2,$0,16
sll  $1,$1,16
sb   $2,2($1)
addiu $2,$2,1
lb   $2,2($1)
jr   $0
