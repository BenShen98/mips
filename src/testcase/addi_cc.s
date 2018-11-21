#add_cc,246,,,math exception return -10
lui  $3,32767 #0x7FFF0000
ori $3,$3,65535
addi  $2,$3,1
jr   $0
