#ori_cc,3,,,ori should not sign extend
addiu $4,$0,0x7FFF
ori  $2,$4,0xFFFF
srl $2,$2,14
jr $0
