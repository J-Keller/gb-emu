//
// Created by viking on 11/27/22.
//

#include <fstream>
#include <vector>
#include <chrono>
#include <iostream>
#include <thread>
#include <iomanip>
#include "CPU.h"


CPU::CPU() {
    std::ifstream program("../roms/dmg_boot.bin", std::ios::binary);
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(program), {});
    std::copy(buffer.begin(), buffer.end(), std::begin(bootRom));
}

void CPU::run() {
    while(true) {
        // TODO: move this
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    return;
            }
        }

        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        uint8_t cycles = executeInstruction();
        if (cycles == 0) {
            // invalid / unimplemented instruction
            return;
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        // TODO: Check type
        unsigned int delta = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        //std::cout << std::dec << delta << std::endl;
        waitCycles(cycles, delta);
    }
}

uint8_t CPU::readRegister(uint16_t reg, BytePosition bytePos) {
    switch (bytePos) {
        case LOW:
            // return low byte from register
            return reg & 0x00FF;
        case HIGH:
            // return high byte from register
            return reg >> 8;
        default:
            return 0;
    }
}

void CPU::writeRegister(uint16_t &reg, uint8_t value, BytePosition bytePos) {
    switch (bytePos) {
        case LOW:
            // write to low byte from register
            reg = (reg & 0xFF00) | value;
            break;
        case HIGH:
            // write to high byte from register
            reg = (value << 8) | (reg & 0x00FF);
            break;
    }
}

bool CPU::readFlag(Flag flag) const {
    switch (flag) {
        case Z:
            return (af >> 7) & 1;
        case N:
            return (af >> 6) & 1;
        case H:
            return (af >> 5) & 1;
        case C:
            return (af >> 4) & 1;
        default:
            return false;
    }
}

void CPU::writeFlag(Flag flag, bool value) {
    switch (flag) {
        case Z:
            af |= 1 << 7;
        case N:
            af |= 1 << 6;
        case H:
            af |= 1 << 5;
        case C:
            af |= 1 << 4;
    }
}

void CPU::waitCycles(double cycles, double offset) const {
    std::this_thread::sleep_for(std::chrono::nanoseconds((int)((cycles * cycleTime) - offset)));
}

uint8_t CPU::readByteFromMemory(uint16_t address) {
    return memory.readByte(address);
}

void CPU::writeByteToMemory(uint8_t value, uint16_t address) {
    memory.writeByte(value, address);
}

uint8_t CPU::readInstruction(uint16_t &programCounter) {
    uint8_t instruction;
    if (programCounter < 0x100) {
        // read from boot rom
        instruction = bootRom[programCounter];
    } else {
        instruction = memory.readByte(programCounter);
    }
    programCounter++;
    return instruction;
}

uint8_t CPU::readImmediate8BitData(uint16_t &programCounter) {
    return readInstruction(programCounter);
}

uint16_t CPU::readImmediate16BitData(uint16_t &programCounter) {
    uint16_t a = readInstruction(programCounter);
    uint8_t b = readInstruction(programCounter);
    return (a << 8) | b;
}

// TODO: check if signed returns cause problems with unsigned readInstruction
int8_t CPU::readImmediate8BitSignedData(uint16_t &programCounter) {
    return readInstruction(programCounter);
}

uint8_t CPU::executeInstruction() {
    display.printScreen();
#ifdef DEBUG
    uint16_t registers[6] = {af, bc, de, hl, sp, pc};
    display.printDebugInfo(registers, bootRom, pc);
#endif
    // initialize variables needed in instructions later in switch statement
    uint8_t result; // result of arithmetic instruction before it is saved to register
    uint8_t tmp1; // one side of arithmetic function
    uint8_t tmp2; // one side of arithmetic function

    // TODO: I didn't even start yet but this needs optimization later
    uint8_t instruction = readInstruction(pc);
    std::cout << std::setw(2) << std::setfill('0') << std::hex << +instruction << std::endl;

    switch(instruction) {
        case 0x00: // nop
            return 4; // no operation -> takes 4 cpu cycles
        case 0x01: // ld BC,d16
            bc = readImmediate16BitData(pc);
            return 12;
        case 0x02: // ld (BC),A
            writeByteToMemory(readRegister(af, HIGH), bc);
            return 8;
        case 0x03:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x04:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x05:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x06:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x07:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x08:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x09:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x0A: // ld A,(BC)
            writeRegister(af, readByteFromMemory(bc), HIGH);
            return 8;
        case 0x0B:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x0C:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x0D:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x0E:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x0F:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x10: // stop
            if (readInstruction(pc) != 0x00) {
                std::cerr << "Invalid STOP instruction" << std::endl;
                // TODO: throw exception
            }
            return 4;
        case 0x11:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x12: // ld (DE),A
            writeByteToMemory(readRegister(af, HIGH), de);
            return 8;
        case 0x13:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x14:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x15:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x16:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x17:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x18:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x19:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x1A: // ld A,(DE)
            writeRegister(af, readByteFromMemory(de), HIGH);
            return 8;
        case 0x1B:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x1C:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x1D:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x1E:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x1F:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x20: // jr NZ,r8
            if (readFlag(N) && readFlag(Z)) {
                pc += readImmediate8BitSignedData(pc);
                return 12;
            }
            return 8;
        case 0x21: // ld hl,d16
            hl = readImmediate16BitData(pc);
            return 12;
        case 0x22: // ldi (HL),A
            writeByteToMemory(readRegister(af, HIGH), hl++);
            return 8;
        case 0x23:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x24:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x25:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x26:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x27:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x28:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x29:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x2A: // ldi A,(HL)
            writeRegister(af, readByteFromMemory(hl++), HIGH);
            return 8;
        case 0x2B:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x2C:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x2D:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x2E:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x2F:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x30:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x31: // ld SP,d16
            sp = readImmediate16BitData(pc);
            return 12;
        case 0x32: // ldd (HL),A
            writeByteToMemory(readRegister(af, HIGH), hl--);
            return 8;
        case 0x33:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x34:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x35:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x36: // ld (HL),n
            return 12;
        case 0x37:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x38:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x39:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x3A: // ldd A,(HL)
            writeRegister(af, readByteFromMemory(hl--), HIGH);
            return 8;
        case 0x3B:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x3C:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x3D:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x3E:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x3F:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x40:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x41:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x42:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x43:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x44:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x45:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x46:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x47:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x48:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x49:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x4A:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x4B:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x4C:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x4D:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x4E:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x4F:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x50:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x51:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x52:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x53:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x54:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x55:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x56:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x57:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x58:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x59:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x5A:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x5B:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x5C:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x5D:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x5E:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x5F:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x60:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x61:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x62:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x63:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x64:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x65:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x66:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x67:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x68:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x69:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x6A:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x6B:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x6C:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x6D:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x6E:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x6F:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x70:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x71:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x72:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x73:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x74:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x75:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x76: // halt
            // TODO: halt
            return 4;
        case 0x77:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x78:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x79:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x7A:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x7B:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x7C: // ld A,H
            writeRegister(af, readRegister(hl, HIGH), HIGH);
            return 4;
        case 0x7D:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x7E:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x7F:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x80:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x81:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x82:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x83:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x84:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x85:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x86:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x87:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x88:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x89:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x8A:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x8B:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x8C:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x8D:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x8E:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x8F:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x90:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x91:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x92:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x93:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x94:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x95:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x96:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x97:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x98:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x99:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x9A:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x9B:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x9C:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x9D:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x9E:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x9F:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0xA0:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA1:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA2:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA3:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA4:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA5:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA6:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA7:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA8:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA9:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xAA:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xAB:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xAC:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xAD:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xAE:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xAF: // xor A
            writeRegister(af, 0, HIGH); // TODO: check if it is really always 0
            writeFlag(Z, true);
            return 4;

        case 0xB0:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB1:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB2:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB3:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB4:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB5:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB6:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB7:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB8:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB9:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xBA:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xBB:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xBC:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xBD:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xBE:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xBF:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0xC0:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC1:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC2:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC3:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC4:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC5:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC6:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC7:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC8:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC9:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xCA:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xCB: // prefix cb -> execute from cb instruction set
            return executeCBInstruction();
        case 0xCC:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xCD:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xCE:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xCF:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0xD0:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD1:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD2:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD3:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD4:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD5:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD6:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD7:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD8:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD9:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xDA:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xDB:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xDC:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xDD:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xDE:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xDF:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0xE0: // ld (FF00+n),A
            writeByteToMemory(readRegister(af, HIGH), 0xFF00 | readImmediate8BitData(pc));
            return 12;
        case 0xE1:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE2: // ld (FF00+C),A
            writeByteToMemory(readRegister(af, HIGH), 0xFF00 | readRegister(bc, LOW));
            return 8;
        case 0xE3:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE4:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE5:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE6:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE7:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE8:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE9:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xEA: // ld (nn),A
            writeByteToMemory(readRegister(af, HIGH), readImmediate16BitData(pc));
            return 16;
        case 0xEB:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xEC:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xED:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xEE:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xEF:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0xF0: // ld A,(FF00+n)
            writeRegister(af, readByteFromMemory(0xFF00 | readImmediate8BitData(pc)), HIGH);
            return 12;
        case 0xF1:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF2: // ld A,(FF00+C)
            writeRegister(af, readByteFromMemory(0xFF00 | readRegister(bc, LOW)), HIGH);
            return 8;
        case 0xF3: // di -> disable interrupts
            ime = 0;
            return 4;
        case 0xF4:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF5:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF6:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF7:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF8:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF9:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xFA: // ld A,(nn)
            writeRegister(af, readByteFromMemory(readImmediate16BitData(pc)), HIGH);
            return 16;
        case 0xFB: // ei -> enable interrupts
            ime = 1;
            return 4;
        case 0xFC:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xFD:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xFE:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xFF:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        default:
            std::cerr << "Instruction 0x" << std::setw(2) << std::setfill('0') << std::hex << +instruction << " does not exist";
            return 0;
    }

    return 0;
}

uint8_t CPU::executeCBInstruction() {
    // initialize variables needed in instructions later in switch statement
    uint8_t result; // result of arithmetic instruction before it is saved to register
    uint8_t tmp1; // one side of arithmetic function
    uint8_t tmp2; // one side of arithmetic function

    // TODO: I didn't even start yet but this needs optimization later
    uint8_t instruction = readInstruction(pc);
    std::cout << "CB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << std::endl;

    switch(instruction) {
        case 0x00:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x01:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x02:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x03:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x04:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x05:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x06:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x07:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x08:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x09:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x0A:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x0B:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x0C:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x0D:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x0E:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x0F:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x10:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x11:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x12:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x13:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x14:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x15:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x16:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x17:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x18:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x19:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x1A:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x1B:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x1C:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x1D:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x1E:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x1F:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x20:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x21:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x22:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x23:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x24:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x25:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x26:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x27:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x28:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x29:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x2A:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x2B:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x2C:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x2D:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x2E:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x2F:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x30:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x31:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x32:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x33:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x34:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x35:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x36:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x37:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x38:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x39:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x3A:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x3B:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x3C:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x3D:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x3E:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x3F:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x40:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x41:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x42:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x43:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x44:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x45:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x46:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x47:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x48:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x49:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x4A:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x4B:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x4C:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x4D:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x4E:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x4F:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x50:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x51:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x52:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x53:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x54:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x55:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x56:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x57:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x58:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x59:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x5A:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x5B:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x5C:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x5D:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x5E:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x5F:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x60:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x61:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x62:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x63:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x64:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x65:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x66:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x67:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x68:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x69:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x6A:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x6B:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x6C:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x6D:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x6E:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x6F:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x70:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x71:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x72:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x73:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x74:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x75:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x76:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x77:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x78:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x79:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x7A:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x7B:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x7C:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x7D:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x7E:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x7F:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x80:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x81:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x82:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x83:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x84:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x85:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x86:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x87:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x88:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x89:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x8A:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x8B:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x8C:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x8D:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x8E:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x8F:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0x90:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x91:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x92:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x93:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x94:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x95:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x96:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x97:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x98:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x99:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x9A:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x9B:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x9C:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x9D:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x9E:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0x9F:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0xA0:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA1:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA2:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA3:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA4:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA5:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA6:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA7:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA8:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xA9:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xAA:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xAB:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xAC:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xAD:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xAE:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xAF:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0xB0:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB1:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB2:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB3:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB4:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB5:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB6:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB7:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB8:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xB9:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xBA:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xBB:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xBC:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xBD:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xBE:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xBF:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0xC0:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC1:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC2:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC3:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC4:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC5:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC6:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC7:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC8:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xC9:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xCA:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xCB:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xCC:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xCD:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xCE:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xCF:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0xD0:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD1:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD2:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD3:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD4:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD5:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD6:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD7:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD8:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xD9:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xDA:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xDB:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xDC:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xDD:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xDE:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xDF:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0xE0:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE1:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE2:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE3:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE4:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE5:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE6:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE7:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE8:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xE9:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xEA:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xEB:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xEC:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xED:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xEE:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xEF:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;

        case 0xF0:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF1:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF2:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF3:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF4:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF5:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF6:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF7:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF8:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xF9:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xFA:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xFB:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xFC:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xFD:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xFE:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        case 0xFF:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " not implemented yet";
            return 0;
        default:
            std::cerr << "Instruction 0xCB " << std::setw(2) << std::setfill('0') << std::hex << +instruction << " does not exist";
            return 0;
    }

    return 0;
}