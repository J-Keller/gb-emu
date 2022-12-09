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

unsigned char CPU::executeInstruction() {

    return 0;
}

void CPU::waitCycles(double cycles, double offset) {
    std::this_thread::sleep_for(std::chrono::nanoseconds((int)((cycles * cycleTime) - offset)));
}
