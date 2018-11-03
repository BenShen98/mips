#bne,0,,,testing bne
addiu $1,$0,20
addiu $2,$0,2
loop: subu  $1,$1,$2
bne  $1,$0,loop
addu  $2,$0,$1
jr $0
