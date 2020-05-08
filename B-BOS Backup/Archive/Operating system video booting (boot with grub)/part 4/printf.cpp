#include "types.h"
#include "printf.h"

clearScreen::clearScreen(){
    uint16_t* VideoMemory = (uint16_t*)0xb8000;//the memory location of where the computer will start printing char to the screen is 0xb8000
    static uint8_t x = 0, y = 0;
    for(y = 0; y < 25; y++){
        for(x = 0; x < 80;x++){
            VideoMemory[(80*y)+x] = (VideoMemory[(80*y)+x] & 0xFF00) | ' ';
        }
    }
}

printf::printf(char* str){ // This is a printf function, taking the pointer or position of the current char 
    uint16_t* VideoMemory = (uint16_t*)0xb8000;//the memory location of where the computer will start printing char to the screen is 0xb8000
    static uint8_t x = 0, y = 0;

    for(int i = 0; str[i] != '\0'; ++i){ //Going to loop through the string until it finds a '\0'
        VideoMemory[(80*y)+x] = (VideoMemory[(80*y)+x] & 0xFF00) | str[i]; //it will set the videoMemory var to current char of the string
                                                                /*If we just set it to str[i], it will override the high byte where the color information is stored
                                                                So we need to combine the current High byte (VideoMemory[i] & 0xBB00) This work as comparing VideoMemory[i] with the bitwise symbole & (AND bitwise symbole) So 0xBB-- will stay the same and ox--00 will change by str[i] text colors */
        switch (str[i]){
            case '\n':
                y++;
                x = 0;
                break;
            default:
                VideoMemory[(80*y)+x] = (VideoMemory[(80*y)+x] & 0xFF00) | str[i];
                x++;
                break;
        }
        if(x >= 80){
            y++;
            x = 0;            
        }
        if(y >= 25){
            clearScreen();
            x = 0;
            y = 0;
        }
    }
}

