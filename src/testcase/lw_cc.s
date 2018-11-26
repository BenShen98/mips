#lw,245,,,mem exception addr $?245
lui $14,8192 #0x20000000
sw $14,0($14)
lw  $2,3($14)
jr  $0
