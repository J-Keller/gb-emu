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
    void printDebugInfo(unsigned short* registers, unsigned char* memory, unsigned short pc);
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

    unsigned short width = 160;
    unsigned short height = 144;
    // TODO: add setting
    unsigned char scale = 3;
};


#endif //GB_EMU_DISPLAY_H
