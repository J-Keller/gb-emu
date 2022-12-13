//
// Created by viking on 11/27/22.
//

#include "Memory.h"


void Memory::write(std::vector<unsigned char> buffer, unsigned short address) {
    std::copy(buffer.begin(), buffer.end(), std::begin(ram) + address);
}

unsigned char Memory::readByte(unsigned short address) {
    return ram[address];
}

void Memory::writeByte(unsigned char value, unsigned short address) {
    ram[address] = value;
}