C_SOURCES = $(wildcard boot/*.c kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.c drivers/*c)

OBJ = ${C_SOURCES:.c=.o}
KER=/home/bradon/Desktop/Operating_system_2.0/Operating_system_video
all: os-image

run: all
	bochs

os-image: boot_main.bin #kernel.bin
	cat $^ > os-image

kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -o $@ -Ttext 0x10000 $^ --oformat binary
	
deploy: 
	cd $(KER) && make kernel.o
%.o : %.c ${HEADERS}
	gcc -ffreestanding -c $< -o $@;#freestanding is for the compiler to know it is compiling the kernel.

%.o : %.asm
	nasm $< -f elf -o $@

%.bin : %.asm
	nasm -f bin $< -o $@

clean:
	rm -fr *.bin *.dis *.o os-image
	rm -fr kernel/*.o boot/*.bin drivers/*.o
	
#nasm -f bin boot_sect_simple.asm -o boot_sect_simple.bin
#qemu-system-x86_64 <filename>
