#most of the information here is from the http://www.lowlevel.eu/wiki/Tyndur
.set IRQ_BASE, 20 #look up why we need this

.section .text #code section

.extern _ZN16InterruptManager15handleInterruptEhj #use "nm interrupt.o" this is the function handlerInterrupt(uint8_t interruptNumber, uint32_t esp)
.global _ZN16InterruptManager22IgnoreInterruptRequestEv #static void IgnoreInterruptRequest();


.macro HandleException num #a macro to make the handler list with the num variables
.global _ZN16InterruptManager16HandleException\num\()Ev#this creates the implmentation of the 
_ZN16InterruptManager16HandleException\num\()Ev: #set location of marcro jump
    movb $\num, (interruptNumber) #set the interrupt number with the variable in the marco
    jmp int_bottom #jump to int_bottom
    call _ZN16InterruptManager22IgnoreInterruptRequestEv
.endm #end macro


.macro HandleInterruptRequest num #a macro to make the handler list with the num variables
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev#this creates the implmentation of the HandlerInterruptRequest
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptNumber) #set the interrupt number with the variable in the marco
    jmp int_bottom #jump to int_bottom
.endm #end macro

#for more interrupts we will just copy this many times

HandleInterruptRequest 0x00;

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
    call _ZN16InterruptManager15handleInterruptEhj
    movl %eax, %esp #move stack pointer to eax pointer
    
    
    #restore the old register values (becuase it is in a stack it needs to be in an oppossite value)
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    
    call _ZN16InterruptManager22IgnoreInterruptRequestEv
    
_ZN16InterruptManager22IgnoreInterruptRequestEv:
    iret #return
    
    
.section .data
    interruptNumber: .byte 0 #set interruptnumber to 0
    
#resume at 20:00
