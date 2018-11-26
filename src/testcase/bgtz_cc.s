#bgtz,1,,,test branch when =0 
addiu $3,$0,0xFFFF #-1
addiu $4,$0,1
loop: add  $3,$3,$4
bgtz  $3,loop
add $2,$3,$4
jr $0
