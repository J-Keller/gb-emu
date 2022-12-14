//
// Created by viking on 11/27/22.
//

#include <fstream>
#include <vector>
#include <chrono>
#include <iostream>
#include <thread>
#include "CPU.h"


CPU::CPU() {
    std::ifstream program("../roms/dmg_boot.bin", std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(program), {});
    std::copy(buffer.begin(), buffer.end(), std::begin(bootRom));
}

void CPU::run() {
    while(true) {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        unsigned char cycles = executeInstruction();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        // TODO: Check type
        unsigned int delta = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << delta << std::endl;
        waitCycles(cycles, delta);
        break;
    }
}

unsigned char CPU::readRegister(unsigned short reg, BytePosition bytePos) {
    switch (bytePos) {
        case LOW:
            // return low byte from register
            return reg & 0x00FF;
        case HIGH:
            // return high byte from register
            return reg >> 8;
    }
}

void CPU::writeRegister(unsigned short &reg, unsigned char value, BytePosition bytePos) {
    switch (bytePos) {
        case LOW:
            // write to low byte from register
            reg = (reg & 0xFF00) | value;
            break;
        case HIGH:
            // write to high byte from register
            reg = (value << 8) | (reg & 0x00FF);
            break;
    }
}

void CPU::waitCycles(double cycles, double offset) const {
    std::this_thread::sleep_for(std::chrono::nanoseconds((int)((cycles * cycleTime) - offset)));
}

unsigned char CPU::readByteFromMemory(unsigned short address) {
    return memory.readByte(address);
}

void CPU::writeByteToMemory(unsigned char value, unsigned short address) {
    memory.writeByte(value, address);
}

unsigned char CPU::readInstruction(unsigned short &programCounter) {
    unsigned char instruction;
    if (programCounter < 0x100) {
        // read from boot rom
        instruction = bootRom[programCounter];
    } else {
        instruction = memory.readByte(programCounter);
    }
    programCounter++;
    return instruction;
}

unsigned char CPU::executeInstruction() {
    // TODO: I didn't even start yet but this needs optimization later

    // pre initialize variables that might be used in instructions (second / third byte)
    unsigned char n1;
    unsigned char n2;
    unsigned short nn;
    signed char d1;

    unsigned char instruction8Bit = readInstruction(pc);

    // CPU control instructions
    switch (instruction8Bit) {
        case 0x00: // nop
            return 4; // no operation -> takes 4 cpu cycles
        case 0x76: // halt
            // TODO: halt
            return 4;
        case 0x10: // stop
            if (readInstruction(pc) != 0x00) {
                std::cerr << "Invalid STOP instruction" << std::endl;
                // TODO: throw exception
            }
            return 4;
        case 0xF3: // di -> disable interrupts
            ime = 0;
            return 4;
        case 0xFB: // ei -> enable interrupts
            ime = 1;
            return 4;
        case 0xCB: // prefix cb -> execute from cb instruction set
            // TODO: cb instructions
            return 4;
        default:
            break;
    }

    // 8 bit load instructions
    switch (instruction8Bit) {
        case 0x3A: // ldd A,(HL)
            writeRegister(af, readByteFromMemory(hl--), HIGH);
            return 8;
        case 0x32: // ldd (HL),A
            writeByteToMemory(readRegister(af, HIGH), hl--);
            return 8;
        case 0x2A: // ldi A,(HL)
            writeRegister(af, readByteFromMemory(hl++), HIGH);
            return 8;
        case 0x22: // ldi (HL),A
            writeByteToMemory(readRegister(af, HIGH), hl++);
            return 8;
        case 0xE2: // ld (FF00+C),A
            writeByteToMemory(readRegister(af, HIGH), 0xFF00 | readRegister(bc, LOW));
            return 8;
        case 0xF2: // ld A,(FF00+C)
            writeRegister(af, readByteFromMemory(0xFF00 | readRegister(bc, LOW)), HIGH);
            return 8;
        case 0xE0: // ld (FF00+n),A
            n1 = readInstruction(pc);
            writeByteToMemory(readRegister(af, HIGH), 0xFF00 | n1);
            return 12;
        case 0xF0: // ld A,(FF00+n)
            n1 = readInstruction(pc);
            writeRegister(af, readByteFromMemory(0xFF00 | n1), HIGH);
            return 12;
        case 0xEA: // ld (nn),A
            n1 = readInstruction(pc);
            n2 = readInstruction(pc);
            nn = (n1 << 8) | n2;
            writeByteToMemory(readRegister(af, HIGH), nn);
            return 16;
        case 0x12: // ld (DE),A
            writeByteToMemory(readRegister(af, HIGH), de);
            return 8;
        case 0x02:// ld (BC),A
            writeByteToMemory(readRegister(af, HIGH), bc);
            return 8;
        case 0xFA: // ld A,(nn)
            n1 = readInstruction(pc);
            n2 = readInstruction(pc);
            nn = (n1 << 8) | n2;
            writeRegister(af, readByteFromMemory(nn), HIGH);
            return 16;
        case 0x1A: // ld A,(DE)
            writeRegister(af, readByteFromMemory(de), HIGH);
            return 8;
        case 0x0A: // ld A,(BC)
            writeRegister(af, readByteFromMemory(bc), HIGH);
            return 8;
        case 0x36: // ld (HL),n
            return 12;
        default:
            break;
    }

    // TODO: CB instructions

    return 0;
}
