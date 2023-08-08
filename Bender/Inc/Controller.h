#pragma once

#include "main.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "IS31FL3246.h"
#include "Bender.h"
#include "Metronome.h"
#include "task_sequence_handler.h"

class Controller
{
public:
    Controller(IS31FL3246 *leds_ptr, Metronome *metro_ptr, Bender *chan_a, Bender *chan_b, Bender *chan_c, Bender *chan_d) : touchInterrupt(TOUCH_INT, PullUp), rec_led(REC_LED), clear_led(CLEAR_LED), reset_led(RESET_LED)
    {
        leds = leds_ptr;
        metro = metro_ptr;
        channels[0] = chan_a;
        channels[1] = chan_b;
        channels[2] = chan_c;
        channels[3] = chan_d;
    }

    IS31FL3246 *leds;
    InterruptIn touchInterrupt; // interupt pin for MPR121
    DigitalOut rec_led;
    DigitalOut clear_led;
    DigitalOut reset_led;

    Metronome *metro;
    Bender *channels[4];

    void init();
    void handlePulse(uint8_t pulse);
    void handleClockCorrect(uint8_t pulse);
    void handleBarReset();
    void handleStepCallback(uint16_t step);
};