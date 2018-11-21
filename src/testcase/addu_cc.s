#addu,0,,,simple addu corner case
lui  $3,32767 #0x7FFF0000
ori $3,$3,65535
addu  $2,$3,1
jr   $0
