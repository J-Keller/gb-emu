//
// Created by viking on 11/27/22.
//

#include <fstream>
#include <vector>
#include "CPU.h"


CPU::CPU() {
    std::ifstream program("../roms/dmg_boot.bin", std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(program), {});
    std::copy(buffer.begin(), buffer.end(), std::begin(bootRom));
}