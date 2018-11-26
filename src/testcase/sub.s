#sub,254,,,simple sub should return -2
addiu $14,$0,65535
#this is -1
li $4,1
sub  $2,$14,$4
jr    $0
