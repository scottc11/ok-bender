#pragma once

#include "main.h"
#include "DigitalOut.h"
#include "AnalogHandle.h"
#include "IS31FL3246.h"
#include "Bender.h"
#include "Sequence.h"
#include "Ratchet.h"

class Channel {
public:
    Channel(int _index, IS31FL3246 *leds_ptr, int led_pin, PinName trig_pin, Bender *bender_ptr) : trig_out(trig_pin, 1)
    {
        index = _index;
        led_driver = leds_ptr;
        trig_led_pin = led_pin;
        bender = bender_ptr;
    }

    int index;
    int trig_led_pin;
    DigitalOut trig_out; // inverted output via transistor
    IS31FL3246 *led_driver;
    Bender *bender;
    Sequence sequence;
    Ratchet ratchet;

    void init();
    void handlePulse(int pulse);
    void setTrigLed(int pwm);
    void benderActiveCallback(uint16_t value);
    void benderIdleCallback();
    void ratchetHandler(bool state);
};