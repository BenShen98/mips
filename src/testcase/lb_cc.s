#lb,0,,,check loading signed or not
lui  $3,8192 #0x2000 0000
addiu $2,$0,65535 #-1
sw   $2,0($3)
lb   $2,2($3)
addi $2,$2,1 #-1+1=0
jr   $0
