#srl,255,,,testing logical right shift
lui $1,65535
addiu $3,$0,17
srl $2,$1,$3
jr $0
