#add_cc1,246,,,negative overflow should return 246
lui $3,0x7000
lui $4,0x7000
add $2,$3,$4
jr   $0
