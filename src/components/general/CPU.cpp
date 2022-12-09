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
        auto cycles = executeInstruction();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        unsigned int delta = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << delta << std::endl;
        waitCycles(cycles, delta);
        break;
    }
}

void CPU::waitCycles(double cycles, double offset) {
    std::this_thread::sleep_for(std::chrono::nanoseconds((int)((cycles * cycleTime) - offset)));
}

unsigned char CPU::executeInstruction() {
    // TODO: I didn't even start yet but this needs optimization later
    // stick to boot rom for now
    unsigned char instruction8Bit = bootRom[pc];

    // CPU control instructions
    switch (instruction8Bit) {
        case 0x00:
            return 4; // no operation -> takes 4 cpu cycles
        case 0x76:
            // halt -> until interrupt
            break;
        case 0x10:
            // stop -> validate next 8 bits are 0x00
            break;
        case 0xF3:
            ime = 0; // di -> disable interrupts
            break;
        case 0xFB:
            ime = 1; // ei -> enable interrupts
            break;
        case 0xCB:
            // prefix cb -> execute from cb instruction set
            break;
        default:
            break;
    }

    unsigned char leftNibble = (instruction8Bit >> 4) & 0x0F;
    unsigned char rightNibble = instruction8Bit & 0x0F;

    if (leftNibble >= 0x4 && leftNibble <= 0x7) {
        // load instruction
    }
    if (leftNibble >= 0x8 && leftNibble <= 0xB) {
        // arithmetic instruction
    }

    // TODO: CB instructions

    return 0;
}
