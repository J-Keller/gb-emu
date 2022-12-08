#include <iostream>
#include <thread>
#include "components/general/CPU.h"

int main(int argc, char* argv[]) {
    CPU cpu;
    // cpu.loadProgram(argv[1]) // TODO: implement

    std::this_thread::sleep_for(std::chrono::seconds (5));
    std::cout << "Hello World!" << std::endl;
}
