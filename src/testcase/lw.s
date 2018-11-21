#lw,0,,,testing loadword
lui $14,8192 #0x20000000
sw $14,0($14)
lw  $2,0($14)
jr  $0
