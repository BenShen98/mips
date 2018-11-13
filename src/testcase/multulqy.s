#multulqy,65343,,,comment
addiu $t0, $t0,3
sll $t0,$t0,28


lui $t1,0x8000
lui $t2,0x7FFF
multu  $t1, $t2
mfhi  $31
mflo $t3

sw $t3, 4($t0)
srl $t3,$t3,8
sw $t3, 4($t0)
srl $t3,$t3,8
sw $t3, 4($t0)
srl $t3,$t3,8
sw $t3, 4($t0)


sw $31, 4($t0)
srl $31,$31,8
sw $31, 4($t0)
srl $31,$31,8
sw $31, 4($t0)
srl $31,$31,8
sw $31, 4($t0)
jr   $0
