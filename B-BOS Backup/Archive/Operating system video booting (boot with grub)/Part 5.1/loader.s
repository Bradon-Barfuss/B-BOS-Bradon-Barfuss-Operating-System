.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO /*look up flag info*/
.set MAGIC, 0x1BADB002 /*the magic number for the kernel*/
.set CHECKSUM, -(MAGIC + FLAGS);/* Look up what is checksum*/

.section .multiboot /*look up information about .multiboot section*/
    .align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM
    
    
.section .text /*where the code goes*/
.extern kernelMain
.global _loader
.type _loader, @function; #allows to be accessed from the outside
mov $stack_top, %esp; /*The '$' means use the hex value of the number or immediate (constands) Move esp (stack pointer) register into the kernel_stack location*/
pushl %eax /*push register (%) into the 32bit ax register*/
pushl %ebx /*push the magic number in the register (%) in the 32bit bx register*/
_loader:    
    call kernelMain
    /*The '%' Means it is a register*/
_stop: /*creates a infinte loop*/
    cli /*cli is a interrupt flag, where you disabled interrupts*/
    hlt /*hlt (halt) stands for stopping the cpu*/
    jmp _stop /*Repeat the cycle*/
                            

.section .bss /*.bss is where undifine variables are at*/
.align 16
stack_bottom:
.skip 16384 #16 KB
stack_top:

kernel_stack:
