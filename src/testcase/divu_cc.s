#divu,133,,,Q  5368709
addiu $14,$0,0xffff # the immediate is sign extend to 0xffffffff
addiu $3,$0,800
divu $14,$3
mfhi $14
mflo $2
jr $0
