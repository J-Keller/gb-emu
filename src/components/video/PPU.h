//
// Created by viking on 1/7/23.
//

#ifndef GB_EMU_PPU_H
#define GB_EMU_PPU_H


#include <cstdint>
#include "../general/Memory.h"

class PPU {
public:
    void run();
    void shutdown();
private:
    bool isRunning = true;

    static const uint16_t lcdcAddress = 0xFF40;
    uint8_t lcdc = 0; // LCD Control
    Memory memory;

    void readLcdc();
    void writeLcdc(uint8_t value);
};


#endif //GB_EMU_PPU_H
