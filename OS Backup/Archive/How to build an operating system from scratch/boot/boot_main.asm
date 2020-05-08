[org 0x7c00] ; The same one we used when linking the kernel
KERNEL_OFFSET db 0x10000
[bits 16]
    real_start:
    mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot
    mov bp, 0x9000
    mov sp, bp

    ;mov bx, MSG_REAL_MODE 
    ;call printf

    call load_kernel ; read the kernel from disk
    call switch_to_pm ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'
    jmp $ ; Never executed

%include "/home/bradon/OS/boot/boot_disk.asm"
%include "/home/bradon/OS/boot/bootprint.asm"
%include "/home/bradon/OS/boot/gdt.asm"
%include "/home/bradon/OS/boot/print_PM.asm"
%include "/home/bradon/OS/boot/switch_to_pm.asm"
%include "/home/bradon/OS/boot/kernel_entry.asm"
%include "/home/bradon/OS/OSV"

[bits 16]
load_kernel:
    ;mov bx, MSG_LOAD_KERNEL
    ;call printf
    ;call print_nl

    mov bx, KERNEL_OFFSET ; Read from disk and store in 0x10000
    mov dh, 2
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
START_PM:
    ;mov ebx, MSG_PROT_MODE
    call KERNEL_OFFSET
    call kernel_entry
    jmp $ ; Stay here when the kernel returns control to us (if ever)


BOOT_DRIVE db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
MSG_REAL_MODE db "Started in 16-bit", 0
MSG_PROT_MODE db "Landed in 32-bit", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0

; padding
times 510 - ($-$$) db 0
dw 0xaa55
