#include <iostream>
#include <thread>
#include "components/video_display/Display.h"

int main() {
    Display display;
    display.printScreen();
    std::this_thread::sleep_for(std::chrono::seconds (5));
    std::cout << "Hello World!" << std::endl;
}
