[bits 32]; set 32 bit mode

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_string_pm:
   pusha ; push all registers
   mov edx, VIDEO_MEMORY ; set edx as start of video memory

print_string_pm_loop:
   mov al, [ebx] ; store the char at EBX in AL
   mov ah, WHITE_ON_BLACK ; store the attributes in AH

   cmp al, 0 ; compare al with 0
   je print_string_pm_done

   mov [edx], ax ; I believe this enables us to add to edx
                 ; position
   add ebx, 1 ; increase the char location
   add edx, 2 ; move to the next char in cell in video memory

   jmp print_string_pm_loop ; start at the beginning of loop

print_string_pm_done:
   popa
   ret


