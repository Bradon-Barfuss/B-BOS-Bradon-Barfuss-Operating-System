.code16
.text   
    .globl _start
    
_start:
    . = _start + 510 #move to the 510th bytes from position 0
    .byte 0x55 #apend to the end of 510 bytes
    .byte 0xaa
    
    
