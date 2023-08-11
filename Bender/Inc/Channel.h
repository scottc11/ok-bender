#pragma once

#include "main.h"
#include "AnalogHandle.h"
#include "IS31FL3246.h"
#include "Bender.h"
#include "Sequence.h"

class Channel {
public:
    Channel(int _index, IS31FL3246 *leds_ptr, int led_pin, PinName pot_adc_pin, Bender *bender_ptr) : pot_adc(pot_adc_pin)
    {
        index = _index;
        led_driver = leds_ptr;
        trig_led_pin = led_pin;
        bender = bender_ptr;
    }

    int index;
    int trig_led_pin;
    IS31FL3246 *led_driver;
    AnalogHandle pot_adc;
    Bender *bender;
    Sequence sequence;

    void init();
    void handlePulse(int pulse);
    void setTrigLed(int pwm);
    void benderActiveCallback(uint16_t value);
    void benderIdleCallback();
};