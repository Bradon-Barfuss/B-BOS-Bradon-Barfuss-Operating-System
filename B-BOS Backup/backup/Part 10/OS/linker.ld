/*This is the linker file, the linker will take muiltable .o or object files and link them into one .bin file. Object files has a three part structure
.text, .data, .bss. The .text section is where the code goes, i.e. the main() program. The .data section is where all the defined variables would go. And the .bss section is where the global undefine variables would go. The linker will sperate each section of the mulitable files and combine them into each section. 

See Linker Notes for more information
*/

ENTRY(_loader) /*was LOADER tells the linker entry point is the loader function in the loader.s*/
OUTPUT_FORMAT(elf32-i386) /*set up the format of the cpu)*/
OUTPUT_ARCH(i386:i386) /*the cpu archtechure*/

/*This is selecting all the different sections from the .o files, the .text, .data, .bss
the .text section is where the executable code is. The .data section is where all the defined variables are at.
The .bss section is where all the global variables will go.*/
SECTIONS{
    . = 1M; /*The size of .text section is 1*1024*1024 or 1048576*/
    
    .text BLOCK(4K) : ALIGN(4K){/*look up information about the ':' and it the start of the .text (code) section of the linker*/
        *(.multiboot)/*get multiboot section from loader.s(loader.o)*/
        *(.text*)/*get text(code) section from loader.s(loader.o)*/
        *(.rodata)/*look up information*/
    }
        
    .data BLOCK(4K) : ALIGN(4K){/*look up information*/
        start_ctors = .; /*start of construction?*/
        KEEP(*(.int_array ));
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.* )));
        end_ctors = .;
        
        
        *(.data) /*the rest of the data section from the .o files I think*/
    }
    
    .bss BLOCK(4K) : ALIGN(4K){/*.bss section*/
        *(.bss) /*get all the .bss sections (undefine varibles)*/
    }
    
    /DISCARD/ : /*discard information for the linker, fini_array and the comments*/
    {
        *(.fini_array*)
        *(.comment) 
    }
}
