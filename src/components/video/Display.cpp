//
// Created by viking on 11/27/22.
//

#include <iostream>
#include "Display.h"

#ifdef DEBUG
WINDOW *createNewWindow(int width, int height, int xPos, int yPos) {
    WINDOW *local_win;

    local_win = newwin(height, width, yPos, xPos);
    box(local_win, 0 , 0);
    wrefresh(local_win);

    if(local_win == nullptr) {
        std::cerr << "window cannot be created" << std::endl;
    }

    return local_win;
}

void destroyWindow(WINDOW *window) {
    wborder(window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(window);
    delwin(window);
}

void Display::initDebugStuff() {
    // registers
    uint8_t registerWindowWidth = 18;
    uint8_t registerWindowHeight = 8;
    uint8_t registerWindowXPos = 1;
    uint8_t registerWindowYPos = 1;

    registerWindow = createNewWindow(registerWindowWidth, registerWindowHeight, registerWindowXPos, registerWindowYPos);

    // pc
    uint8_t pcWindowWidth = 19;
    uint8_t pcWindowHeight = 3;
    uint8_t pcWindowXPos = 1;
    uint8_t pcWindowYPos = 9;

    pcWindow = createNewWindow(pcWindowWidth, pcWindowHeight, pcWindowXPos, pcWindowYPos);

    // TODO: stack
}

void Display::printDebugInfo(uint16_t *registers, uint8_t* memory, uint16_t pc) {
    // registers & flags
    mvwprintw(registerWindow, 0, 0, "registers");
    // registers
    mvwprintw(registerWindow, 1, 1, "AF: 0x%.4X", registers[0]);
    mvwprintw(registerWindow, 2, 1, "BC: 0x%.4X", registers[1]);
    mvwprintw(registerWindow, 3, 1, "DE: 0x%.4X", registers[2]);
    mvwprintw(registerWindow, 4, 1, "HL: 0x%.4X", registers[3]);
    mvwprintw(registerWindow, 5, 1, "SP: 0x%.4X", registers[4]);
    mvwprintw(registerWindow, 6, 1, "PC: 0x%.4X", registers[5]);
    // flags
    mvwprintw(registerWindow, 1, 13, "Z: %d", (registers[0] >> 7) & 1);
    mvwprintw(registerWindow, 2, 13, "N: %d", (registers[0] >> 6) & 1);
    mvwprintw(registerWindow, 3, 13, "H: %d", (registers[0] >> 5) & 1);
    mvwprintw(registerWindow, 4, 13, "C: %d", (registers[0] >> 4) & 1);
    wrefresh(registerWindow);

    // currently executed code
    mvwprintw(pcWindow, 0, 0, "program");
    mvwprintw(pcWindow, 1, 1, "-> %.3X %.2X%.2X%.2X%.2X", pc, memory[pc], memory[pc + 1], memory[pc + 2], memory[pc + 3]);
    wrefresh(pcWindow);
}
#endif

void Display::printScreen() {
    SDL_SetRenderDrawColor(renderer, 0, 0,0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
}

Display::Display() {
#ifdef DEBUG
    // init ncurses
    initscr();
    cbreak();
    refresh();

    initDebugStuff();
#endif

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("GameBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * scale, height * scale, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);
}

Display::~Display() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

#ifdef DEBUG
    // destroy debug stuff
    destroyWindow(registerWindow);
    endwin();
#endif
}