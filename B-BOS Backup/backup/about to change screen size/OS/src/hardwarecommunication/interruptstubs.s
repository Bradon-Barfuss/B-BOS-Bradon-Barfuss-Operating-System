#most of the information here is from the http://www.lowlevel.eu/wiki/Tyndur
.set IRQ_BASE, 0x20 #look up why we need this

.section .text #code section

.extern _ZN2OS21hardwarecommunication16InterruptManager15handleInterruptEhj #use "nm interrupt.o" this is the function handlerInterrupt(uint8_t interruptNumber, uint32_t esp)
.global _ZN2OS21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv #static void IgnoreInterruptRequest();


.macro HandleException num #a macro to make the handler list with the num variables
.global _ZN2OS21hardwarecommunication16InterruptManager19HandleException\num\()Ev#this creates the implmentation of the 
_ZN2OS21hardwarecommunication16InterruptManager19HandleException\num\()Ev: #set location of marcro jump
    movb $\num, (interruptNumber) #set the interrupt number with the variable in the marco
    jmp int_bottom #jump to int_bottom
    #call _ZN2OS21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv
.endm #end macro


.macro HandleInterruptRequest num #a macro to make the handler list with the num variables
.global _ZN2OS21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev#this creates the implmentation of the HandlerInterruptRequest
_ZN2OS21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptNumber) #set the interrupt number with the variable in the marco
    jmp int_bottom #jump to int_bottom
.endm #end macro

#for more interrupts we will just copy this many times

 
HandleException 0x00
HandleException 0x01
HandleException 0x02
HandleException 0x03
HandleException 0x04
HandleException 0x05
HandleException 0x06
HandleException 0x07
HandleException 0x08
HandleException 0x09
HandleException 0x0A
HandleException 0x0B
HandleException 0x0C
HandleException 0x0D
HandleException 0x0E
HandleException 0x0F
HandleException 0x10
HandleException 0x11
HandleException 0x12
HandleException 0x13

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01 
HandleInterruptRequest 0x02 
HandleInterruptRequest 0x03 
HandleInterruptRequest 0x04 
HandleInterruptRequest 0x05 
HandleInterruptRequest 0x06 
HandleInterruptRequest 0x07 
HandleInterruptRequest 0x08 
HandleInterruptRequest 0x09 
HandleInterruptRequest 0x0A 
HandleInterruptRequest 0x0B 
HandleInterruptRequest 0x0C 
HandleInterruptRequest 0x0D 
HandleInterruptRequest 0x0E 
HandleInterruptRequest 0x0F
HandleInterruptRequest 0x80 

int_bottom:#is a jump target
    #this code will push the vaules to the stack and jump into the handler
    #but we need to save or store the current registers before we can do the handler
    
    pusha #push all registers
    pushl %ds #push data segment
    pushl %es #push stack
    pushl %fs #look up
    pushl %gs #look up

 
    #this code will push the vaules to the stack and jump into the handler
    pushl %esp #push stack pointer
    push (interruptNumber) #push interrupt number 
    call _ZN2OS21hardwarecommunication16InterruptManager15handleInterruptEhj
    movl %eax, %esp #move stack pointer to eax pointer
    
    
    #restore the old register values (becuase it is in a stack it needs to be in an oppossite value)
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    
    jmp _ZN2OS21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv

_ZN2OS21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv:
    iret #return
    
    
.section .data
    interruptNumber: .byte 0 #set interruptnumber to 0
    
#resume at 20:00
