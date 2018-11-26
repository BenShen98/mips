#add,0,,,checking for immediate signed extension
addiu $3,$0,0xFFFF
add  $2,$3,1
jr   $0
