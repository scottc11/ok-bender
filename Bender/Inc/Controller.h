#pragma once

#include "main.h"
#include "Flash.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "MPR121.h"
#include "IS31FL3246.h"
#include "Channel.h"
#include "Metronome.h"
#include "task_sequence_handler.h"

class Controller
{
public:
    Controller(
        MPR121 *touch_pads_ptr,
        IS31FL3246 *leds_ptr,
        Metronome *metro_ptr,
        Channel *chan_a,
        Channel *chan_b,
        Channel *chan_c,
        Channel *chan_d) : 
            tp_reset(TP_RESET, PullUp),
            ts_btn(TIME_SIG_BTN, PullUp),
            rec_btn(REC_BTN, PullUp),
            reset_btn(RESET_BTN, PullUp),
            clear_btn(CLEAR_BTN, PullUp),
            rec_led(REC_LED),
            clear_led(CLEAR_LED),
            reset_led(RESET_LED)
    {
        touch_pads = touch_pads_ptr;
        leds = leds_ptr;
        metro = metro_ptr;
        channels[0] = chan_a;
        channels[1] = chan_b;
        channels[2] = chan_c;
        channels[3] = chan_d;
    }

    MPR121 *touch_pads;
    IS31FL3246 *leds;

    InterruptIn tp_reset;
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
    int counter;
    int touch_pad_map[4] = {6, 5, 4, 3};
    int time_led_pins[6] = {14, 15, 16, 21, 22, 23};

    void init();
    void onTouch(uint8_t pad);
    void onRelease(uint8_t pad);
    void handleTouchInterrupt();
    void handlePulse(uint8_t pulse);
    void handleClockCorrect(uint8_t pulse);
    void handleClockReset();
    void handleBarReset();
    void handleStepCallback(uint16_t step);

    void handleRecBtn();
    void handleClearBtn();
    void handleResetBtn();
    void handleTsBtn();

    void saveCalibrationDataToFlash();
    void loadCalibrationDataFromFlash();
};