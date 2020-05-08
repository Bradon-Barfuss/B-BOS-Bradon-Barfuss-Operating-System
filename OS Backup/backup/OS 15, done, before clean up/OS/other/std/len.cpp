#include "len.h"
#include "/home/bradon/OS/OSV/types.h"

char len::length(char* str){//my own len function, very cruded, but it works!!!
    int len = 48, tenth = 48, hundrenth = 48; //alinging to the 0 position
    for(int i = 0; str[i] != '\0'; ++i){
        len++;    
    }
    while(len > 57){
        tenth++;
        len = len - 10;
        if(tenth > 57){
            hundrenth++;
            tenth = tenth - 10;
        }
    }
    uint16_t* VideoMemory = (uint16_t*)0xb8000;//the memory location of where the computer will start printing char to the screen is 0xb8000
    static uint8_t x = 0, y = 0;
    VideoMemory[(80*y)+x] = (VideoMemory[(80*y)+x] & 0xFF00) | hundrenth;
    VideoMemory[(80*y)+x+1] = (VideoMemory[(80*y)+x+1] & 0xFF00) | tenth;
    VideoMemory[(80*y)+x+2] = (VideoMemory[(80*y)+x+2] & 0xFF00) | len;

    char clen = (char)len;
    return clen;
}
