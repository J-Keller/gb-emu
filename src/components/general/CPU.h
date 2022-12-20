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

    uint8_t bootRom[0x100] = {};
    std::stack<uint16_t> stack;


    // registers
    uint16_t af = 0; // Accumulator & Flags
    uint16_t bc = 0;
    uint16_t de = 0;
    uint16_t hl = 0;
    uint16_t sp = 0; // Stack Pointer
    uint16_t pc = 0; // Program counter

    // flags
    uint8_t ime = 1; // Interrupt master enable flag

    // other
    const double cycleTime = 238.418579102; // Google calculator result for clock speed might be inaccurate
    enum BytePosition {
        LOW,
        HIGH,
    };
    enum Flag {
        Z,
        N,
        H,
        C,
    };

    // helper functions
    static uint8_t readRegister(uint16_t reg, BytePosition bytePos);
    static void writeRegister(uint16_t &reg, uint8_t value, BytePosition bytePos);
    bool readFlag(Flag flag) const;
    void writeFlag(Flag flag, bool value);
    void setZFlagByResult(uint16_t resultValue);
    void setHFlagByResult(uint16_t resultValue);
    void checkBit(uint8_t bit, uint8_t valueToCheck);

    // cpu functions
    uint8_t executeInstruction();
    uint8_t executeCBInstruction();
    void waitCycles(double cycles, double offset) const;
    uint8_t readByteFromMemory(uint16_t address);
    void writeByteToMemory(uint8_t value, uint16_t address);
    uint8_t readInstruction(uint16_t &programCounter);

    // TODO: maybe use templates or something
    uint8_t readImmediate8BitData(uint16_t &programCounter);
    uint16_t readImmediate16BitData(uint16_t &programCounter);
    int8_t readImmediate8BitSignedData(uint16_t &programCounter);
};


#endif //GB_EMU_CPU_H
