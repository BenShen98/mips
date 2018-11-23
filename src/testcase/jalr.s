#jalr,28,,,test if PC+8 is write before excuate delay branch (NOT AFTER)
lui $8,0x1000
ori $8,0x0018
ORI $2, $0,0x456
OR $0,$0,$0 #nop

ORI $2, $0,666
jalr $2,$8
OR $0,$0,$0 #nop
OR $0,$0,$0 #nop

JR $0
