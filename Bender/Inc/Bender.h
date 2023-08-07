#pragma once

#include "main.h"
#include "AnalogHandle.h"
#include "DAC8554.h"
#include "IS31FL3246.h"

class Bender {
public:
    Bender(int _index, IS31FL3246 *leds_ptr, DAC8554 *dac_ptr, DAC8554::Channel _dac_chan, PinName pot_pin, PinName bender_pin, int led_pin) : pot_adc(pot_pin), bend_adc(bender_pin)
    {
        index = _index;
        leds = leds_ptr;
        dac = dac_ptr;
        dac_chan = _dac_chan;
        trig_led_pin = led_pin;
    }

    int index;
    int trig_led_pin;
    AnalogHandle pot_adc;
    AnalogHandle bend_adc;
    IS31FL3246 *leds;
    DAC8554 *dac;
    DAC8554::Channel dac_chan; // which dac channel to address

    uint16_t currOutput;
    uint16_t prevOutput;

    void init();
    void process();
    void setTrigLed(int pwm);
};