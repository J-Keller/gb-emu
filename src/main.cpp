#include <iostream>
#include <thread>
#include "components/general/CPU.h"
#include "components/video/PPU.h"

int main(int argc, char* argv[]) {
    CPU cpu;
    PPU ppu;
    // cpu.loadProgram(argv[1]) // TODO: implement

    std::thread t_cpu(&CPU::run, &cpu);
    std::thread t_ppu(&PPU::run, &ppu);

    t_cpu.join();
    ppu.shutdown();
    t_ppu.join();
}
