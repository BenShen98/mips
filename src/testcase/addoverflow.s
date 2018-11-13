#addiu,0,,,checking for immediate signed extension
addiu $1,$0,65535
add  $2,$1,1
jr   $0
