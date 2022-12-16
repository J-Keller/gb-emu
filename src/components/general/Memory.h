//
// Created by viking on 11/27/22.
//

#ifndef GB_EMU_MEMORY_H
#define GB_EMU_MEMORY_H


#include <vector>


class Memory {
public:
    void write(std::vector<uint8_t> buffer, uint16_t startAddress);
    uint8_t readByte(uint16_t address);
    void writeByte(uint8_t value, uint16_t address);
protected:
private:
    uint8_t ram[65536] = {};
};


#endif //GB_EMU_MEMORY_H
