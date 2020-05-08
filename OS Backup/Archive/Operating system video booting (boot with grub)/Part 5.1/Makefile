# sudo apt-get install g++ binutils libc6-dev-i386

GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386


OP1=/home/bradon/Desktop/Operating_system/boot
objects = loader.o kernel.o gdt.o Interrupts/interruptstubs.o Interrupts/port.o Interrupts/interrupt.o std/printf.o std/len.o

all: mykernel.iso

%.o: %.cpp
	g++ $(GCCPARAMS) -c $< -o $@

%.o: %.s
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

#os-image2: /home/bradon/Desktop/Operating_system/boot/boot_main.bin mykernel.bin
#	cat $^ > os-image2


mykernel.iso: mykernel.bin
	mkdir iso #creats files
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/ #go to boot dir
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg #set grub settings
	echo 'set defult=0' >> iso/boot/grub/grub.cfg #there is only 1 .iso file, so set the default to zero
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Bradons Operating System" {' >> iso/boot/grub/grub.cfg #echo/put the lines into grub.cfg file
	echo '	multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso #creates the cd image
	rm -rf iso #removed made direstores
	
run: mykernel.iso
	qemu-system-x86_64 mykernel.iso #runs the .iso file

.PHONY: clean
clean:
	rm -f $(objects) mykernel.bin  #deletes all object files and mykernel.bin/iso
#nasm boot_main.asm -f elf64 -o
#as --32 -o loader.o loader.s
#qemu-system-x86_64 <filename>
#g++ -m32 -o kernel.o -c kernel.cpp
#as --32 -o loader.o loader.s
#ld -melf_i386 -T linker.ld -o mykernel.bin loader.o kernel.o

#boot sector ideas:
#if we use cat, we can use boot_sector.bin kernel.bin that will start the boot_sector right after that kernel.bin 
