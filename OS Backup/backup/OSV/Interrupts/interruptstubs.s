#most of the information here is from the http://www.lowlevel.eu/wiki/Tyndur
.section .text #code section

.extern _ZN16InterruptManager16handlerInterruptEhj #use "nm interrupt.o" to find the interupt is called



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
    push (interruptnumber) #push interrupt number
    call _ZN16InterruptManager16handlerInterruptEhj
    mov %eax, %esp #move stack pointer to eax pointer
    
    
    #restore the old register values (becuase it is in a stack it needs to be in an oppossite value)
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa
    
    iret #return
    
    
.section .data
    interruptnumber: .byte 0 #set interruptnumber to 0
    
#resume at 20:00
