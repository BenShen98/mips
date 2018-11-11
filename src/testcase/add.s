#add,0,,,here is for comment
addi $1,$0,65535
sll  $3,$1,15
ori $3,$3,65535
add  $2,$3,1
jr   $0
