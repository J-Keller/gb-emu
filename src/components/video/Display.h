//
// Created by viking on 11/27/22.
//

#ifndef GB_EMU_DISPLAY_H
#define GB_EMU_DISPLAY_H

#include <SDL.h>
#include <ncurses.h>

class Display {
public:
    Display();
    ~Display();

    void printScreen();

#ifdef DEBUG
    void printDebugInfo(uint16_t* registers, uint8_t* memory, uint16_t pc);
#endif
private:

#ifdef DEBUG
    void initDebugStuff();

    // Debug Stuff
    WINDOW *registerWindow;
    WINDOW *pcWindow;
#endif

    SDL_Window *window;
    SDL_Renderer *renderer;

    uint16_t width = 160;
    uint16_t height = 144;
    // TODO: add setting
    uint8_t scale = 3;
};


#endif //GB_EMU_DISPLAY_H
