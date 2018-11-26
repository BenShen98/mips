#jr,245,,,jumping to invaild addr should return -11
lui $3,0x2000
jr $3
