/* The type.h file is used for alinging the correct bit sizes with functions
 * This is needed in hard ware because everything needs to be bit perfect*/

//if this code alreadly is in the ram, don't read it?

#ifndef __TYPES_H
#define __TYPES_H

//this is where we assinge the correct bit values

    // 8 bit info
    typedef char int8_t;
    typedef unsigned char uint8_t; //the u in uint stands for unsigned

    //16 bit info
    typedef short int16_t;
    typedef unsigned short uint16_t;
    
    //32 bit info
    typedef int int32_t;
    typedef unsigned int uint32_t;
    
    //64 bit info
    typedef long long int int64_t;
    typedef unsigned long long int uint64_t;
    
#endif
    
