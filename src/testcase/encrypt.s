#password_encrypting,0,0001020304,0203040506, testing putc and getc
addi $3,$0,65535  #this is -1
addiu $4,$0,2     #this is for encrypting code
lui $6,12288 #0x30000000
addiu $7,$6,4 #0x30000004
loop: lb $5,3($6) #loading byte into reg 5
beq $5,$3,end # terminate if EOF
add $5,$5,$4 #encrypting code now
sb $5,3($7)  #storing into 0x30000007
j loop

end: jr $0
