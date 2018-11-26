#jr,245,,,jumping to putc addr should return -11
lui $3,0x3000
addi $3,$3,4
jr $3
