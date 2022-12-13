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

void CPU::waitCycles(double cycles, double offset) const {
    std::this_thread::sleep_for(std::chrono::nanoseconds((int)((cycles * cycleTime) - offset)));
}

unsigned char CPU::readByteFromMemory(unsigned short address) {
    return memory.readByte(address);
}

void CPU::writeByteToMemory(unsigned char value, unsigned short address) {
    memory.writeByte(value, address);
}

unsigned char CPU::executeInstruction() {
    // TODO: I didn't even start yet but this needs optimization later
    // stick to boot rom for now
    unsigned char instruction8Bit = bootRom[pc];

    // CPU control instructions
    switch (instruction8Bit) {
        case 0x00: // nop
            return 4; // no operation -> takes 4 cpu cycles
        case 0x76: // halt
            // TODO: halt
            return 4;
        case 0x10: // stop
            // TODO: validate next 8 bits are 0x00
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
            af = (readByteFromMemory(hl--) << 8) | (af & 0x00FF);
            return 8;
        case 0x32: // ldd (HL),A
            writeByteToMemory(af >> 8, hl--);
            return 8;
        case 0x2A: // ldi A,(HL)
            af = (readByteFromMemory(hl++) << 8) | (af & 0x00FF);
            return 8;
        case 0x22: // ldi (HL),A
            writeByteToMemory(af >> 8, hl++);
            return 8;
        case 0xE2: // ld (FF00+C),A
            // TODO: write to io-port C (memory FF00+C)
            return 8;
        case 0xF2: // ld A,(FF00+C)
            // TODO: read from io-port C (memory FF00+C)
            return 8;
        case 0xE0: // ld (FF00+n),A
            // TODO: read next byte as n
            // TODO: write to io-port n (memory FF00+n)
            return 12;
        case 0xF0: // ld A,(FF00+n)
            // TODO: read next byte as n
            // TODO: read from io-port n (memory FF00+n)
            return 12;
        default:
            break;
    }

    // TODO: CB instructions

    return 0;
}
