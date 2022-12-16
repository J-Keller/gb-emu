//
// Created by viking on 11/27/22.
//

#include <cstdint>
#include "Memory.h"


void Memory::write(std::vector<uint8_t> buffer, uint16_t address) {
    std::copy(buffer.begin(), buffer.end(), std::begin(ram) + address);
}

uint8_t Memory::readByte(uint16_t address) {
    return ram[address];
}

void Memory::writeByte(uint8_t value, uint16_t address) {
    ram[address] = value;
}