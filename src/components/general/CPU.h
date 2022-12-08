//
// Created by viking on 11/27/22.
//

#ifndef GB_EMU_CPU_H
#define GB_EMU_CPU_H


#include "../video/Display.h"

class CPU {
public:
    CPU();
private:
    Display display;

    unsigned char bootRom[0x100] = {};

    // registers
    unsigned short af = 0; // Accumulator & Flags
    unsigned short bc = 0;
    unsigned short de = 0;
    unsigned short hl = 0;
    unsigned short sp = 0; // Stack Pointer
    unsigned short pc = 0; // Program counter
};


#endif //GB_EMU_CPU_H
