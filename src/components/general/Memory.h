//
// Created by viking on 11/27/22.
//

#ifndef GB_EMU_MEMORY_H
#define GB_EMU_MEMORY_H


#include <vector>


class Memory {
public:
    void write(std::vector<unsigned char> buffer, unsigned short startAddress);
    unsigned char readByte(unsigned short address);
    void writeByte(unsigned char value, unsigned short address);
protected:
private:
    unsigned char ram[65536] = {};
};


#endif //GB_EMU_MEMORY_H
