//
// Created by viking on 11/27/22.
//

#ifndef GB_EMU_CPU_H
#define GB_EMU_CPU_H


#include <stack>
#include "../video/Display.h"
#include "Memory.h"

class CPU {
public:
    CPU();

    void run();

private:
    // components
    Display display;
    Memory memory;

    unsigned char bootRom[0x100] = {};
    std::stack<unsigned short> stack;


    // registers
    unsigned short af = 0; // Accumulator & Flags
    unsigned short bc = 0;
    unsigned short de = 0;
    unsigned short hl = 0;
    unsigned short sp = 0; // Stack Pointer
    unsigned short pc = 0; // Program counter

    // flags
    unsigned char ime = 1; // Interrupt master enable flag

    // other
    const double cycleTime = 238.418579102; // Google calculator result for clock speed might be inaccurate

    // functions
    unsigned char executeInstruction();
    void waitCycles(double cycles, double offset) const;
    unsigned char readByteFromMemory(unsigned short address);
    void writeByteToMemory(unsigned char value, unsigned short address);
};


#endif //GB_EMU_CPU_H
