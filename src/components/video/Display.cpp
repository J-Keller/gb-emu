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
    unsigned char registerWindowWidth = 19;
    unsigned char registerWindowHeight = 10;
    unsigned char registerWindowXPos = 1;
    unsigned char registerWindowYPos = 1;

    registerWindow = createNewWindow(registerWindowWidth, registerWindowHeight, registerWindowXPos, registerWindowYPos);

    // pc
    unsigned char pcWindowWidth = 19;
    unsigned char pcWindowHeight = 56;
    unsigned char pcWindowXPos = 1;
    unsigned char pcWindowYPos = 11;

    pcWindow = createNewWindow(pcWindowWidth, pcWindowHeight, pcWindowXPos, pcWindowYPos);

    // TODO: stack
}

void Display::printDebugInfo(unsigned short *registers, unsigned char* memory, unsigned short pc) {
    // registers
    mvwprintw(registerWindow, 0, 0, "registers");
    mvwprintw(registerWindow, 1, 1, "AF: 0x%.4X", registers[0]);
    mvwprintw(registerWindow, 2, 1, "BC: 0x%.4X", registers[1]);
    mvwprintw(registerWindow, 3, 1, "DE: 0x%.4X", registers[2]);
    mvwprintw(registerWindow, 4, 1, "HL: 0x%.4X", registers[3]);
    mvwprintw(registerWindow, 5, 1, "SP: 0x%.4X", registers[4]);
    mvwprintw(registerWindow, 6, 1, "PC: 0x%.4X", registers[5]);
    wrefresh(registerWindow);

    // currently executed code
    mvwprintw(pcWindow, 0, 0, "program");
    for(char i = -54; i < 54; i += 2) {
        if (i == 0) {
            mvwprintw(pcWindow, 28, 1, "->");
        }
        mvwprintw(pcWindow, (i/2) + 28, 4, "%.3X %.2X%.2X", pc + i, memory[pc + i], memory[pc + i + 1]);
    }
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
    window = SDL_CreateWindow("chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * scale, height * scale, SDL_WINDOW_SHOWN);
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