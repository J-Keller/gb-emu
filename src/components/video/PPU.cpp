//
// Created by viking on 1/7/23.
//

#include <iostream>
#include <thread>
#include "PPU.h"

void PPU::readLcdc() {
    lcdc = memory.readByte(lcdcAddress);
}

void PPU::writeLcdc(uint8_t value) {
    memory.writeByte(value, lcdcAddress);
}

void PPU::run() {
    while(isRunning) {
        // TODO: do ppu stuff
        //  - read VRAM
        //  - fetch pixels / tiles to be rendered
        //  - render to display
        //  - wait until next cycle
    }
}

void PPU::shutdown() {
    isRunning = false;
}
