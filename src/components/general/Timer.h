//
// Created by viking on 1/28/23.
//

#ifndef GB_EMU_TIMER_H
#define GB_EMU_TIMER_H


#include <cstdint>

class Timer {
public:
private:
    // Timer register addresses
    const uint16_t div  = 0xFF04; // Divider register
    const uint16_t tima = 0xFF05; // Timer counter
    const uint16_t tma  = 0xFF06; // Timer modulo
    const uint16_t tac  = 0xFF07; // Timer control
};


#endif //GB_EMU_TIMER_H
