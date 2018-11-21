#addi_cc,246,,,positive overflow return -10
lui  $3,0x8000
addi $4,$0,0x8000
add  $2,$3,$4
jr   $0
