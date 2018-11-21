
ENTRY(_entry)

SECTIONS
{

.text
  li $v0,1
  jal .t

.rt: jr  $0

.t: xori $0,$ra,.rt
  jr $0x

}
