#add,246,,,positive overflow should return 246
lui $3,0x8000
lui $4,0x8000
add $2,$3,$4
jr   $0
