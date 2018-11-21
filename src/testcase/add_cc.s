#add_cc,246,,,positive overflow should return 246
lui $3,0xf000
lui $4,0xf000
add $2,$3,$4
jr   $0
