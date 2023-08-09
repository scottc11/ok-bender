#pragma once

#include "main.h"
#include "Flash.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "IS31FL3246.h"
#include "Channel.h"
#include "Metronome.h"
#include "task_sequence_handler.h"

class Controller
{
public:
    Controller(
        IS31FL3246 *leds_ptr,
        Metronome *metro_ptr,
        Channel *chan_a,
        Channel *chan_b,
        Channel *chan_c,
        Channel *chan_d) : touchInterrupt(TOUCH_INT, PullUp),
                          ts_btn(TIME_SIG_BTN, PullUp),
                          rec_btn(REC_BTN, PullUp),
                          reset_btn(RESET_BTN, PullUp),
                          clear_btn(CLEAR_BTN, PullUp),
                          rec_led(REC_LED),
                          clear_led(CLEAR_LED),
                          reset_led(RESET_LED)
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
    InterruptIn ts_btn;
    InterruptIn rec_btn;
    InterruptIn reset_btn;
    InterruptIn clear_btn;
    DigitalOut rec_led;
    DigitalOut clear_led;
    DigitalOut reset_led;

    Metronome *metro;
    Channel *channels[4];

    bool calibrating;

    void init();
    void handlePulse(uint8_t pulse);
    void handleClockCorrect(uint8_t pulse);
    void handleBarReset();
    void handleStepCallback(uint16_t step);

    void handleRecBtn();
    void handleClearBtn();
    void handleResetBtn();
    void handleTsBtn();

    void saveCalibrationDataToFlash();
    void loadCalibrationDataFromFlash();
};