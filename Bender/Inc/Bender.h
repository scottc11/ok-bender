#pragma once

#include "main.h"
#include "okSemaphore.h"
#include "AnalogHandle.h"
#include "DAC8554.h"
#include "IS31FL3246.h"

#define BENDER_DAC_ZERO (BIT_MAX_16 / 2)
#define DEFAULT_MAX_BEND 50000
#define DEFAULT_MIN_BEND 15000
#define BENDER_HYSTERESIS 300
#define BENDER_NOISE_THRESHOLD 1000

class Bender {
public:
    enum BendState
    {
        BENDING_UP,
        BENDING_DOWN,
        BENDING_IDLE
    };

    Bender(DAC8554 *dac_ptr, DAC8554::Channel _dac_chan, PinName adc_pin) : adc(adc_pin)
    {
        dac = dac_ptr;
        dac_chan = _dac_chan;
    }

    AnalogHandle adc;
    DAC8554 *dac;
    DAC8554::Channel dac_chan; // which dac channel to address

    bool override;
    BendState currState;
    BendState prevState;
    uint16_t currBendPosition;                    // current raw ADC value
    uint16_t processedBend;                       // current scaled ADC output
    uint16_t currOutput;                          // current DAC output
    uint16_t prevOutput;                          // previous DAC output
    uint16_t dacOutputRange = BIT_MAX_16 / 2;     // range in which the DAC can output (in either direction)
    bool invertOutput;                            // whether to invert the output of the DAC based on how the ADC reads the direction of the bender

    void init();
    void process();
    uint16_t calculateOutput(uint16_t value);
    void updateDAC(uint16_t value);
    void calibrate();
    uint16_t read();
    
    bool isIdle();
    uint16_t getIdleValue();
    uint16_t getMaxBend();
    uint16_t getMinBend();
    void setMaxBend(uint16_t value) { adc.setInputMax(value); }
    void setMinBend(uint16_t value) { adc.setInputMin(value); }

    void attachIdleCallback(Callback<void()> func);
    void attachActiveCallback(Callback<void(uint16_t bend)> func);
    void attachTriStateCallback(Callback<void(BendState state)> func);

private:
    Callback<void()> idleCallback;                    // MBED Callback which gets called when the Bender is idle / not-active
    Callback<void(uint16_t bend)> activeCallback;     // MBED Callback which gets called when the Bender is active / being bent
    Callback<void(BendState state)> triStateCallback; // callback which gets called when bender changes from one of three BendState states
};