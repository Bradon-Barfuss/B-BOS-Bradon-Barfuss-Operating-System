[bits 16]
switch_to_pm:
    cli ; 1. disable interrupts
    lgdt [gdt_descriptor] ; 2. load the GDT descriptor which defines protected mode
    mov eax, cr0; to switch to protected mode, we set the first bit of CR0, a special CPU controled register
    or eax, 0x1 ; 3. set 32-bit mode bit in cr0, I think 0x0 -> 16bit and 0x1-> 32bit
    mov cr0, eax
    jmp CODE_SEG:init_pm ; 4. far jump by using a different segment to our 32bit code, this makes the cpu flush the cache

[bits 32]
init_pm:
; we are now using 32-bit instructions
    mov ax, DATA_SEG ; 5. in 32-bit mode, are old registers are pointless, so we point our segment registers to the data selector we defined in GDT
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; 6. update the stack right at the top of the free space
    mov esp, ebp

    jmp START_PM ; 7. Call a well-known label with general 32-bit code may take over to boot the 32-bit OS

    
[bits 16]
    jmp $
