#lb,16,,,testing all loadb and storeb
addiu $14,$0,8192
addiu $2,$0,16
sll  $14,$14,16
sb   $2,2($14)
addiu $2,$2,1
lb   $2,2($14)
jr   $0
