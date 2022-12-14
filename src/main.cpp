#include <iostream>
#include <thread>
#include "components/general/CPU.h"

int main(int argc, char* argv[]) {
    CPU cpu;
    // cpu.loadProgram(argv[1]) // TODO: implement
    cpu.run();
}
