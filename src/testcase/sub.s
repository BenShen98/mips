#sub,246,,, overflow => math exception
addiu $1,$0,65535
sll  $3,$1,15
ori $3,$3,65535
addu  $2,$3,1
sub  $2,$2,1
jr    $0
