#include "Controller.h"

int TIME_LED_PINS[6] = {14, 15, 16, 21, 22, 23};

void Controller::init() {
    rec_led.write(1);
    clear_led.write(1);
    reset_led.write(1);

    leds->init();
    leds->setGlobalCurrent(100, 100, 100);
    leds->setControlRegister(false, false, IS31FL3246::PWM_Frequency::_64kHz, false);
    for (int i = 1; i <= 36; i++)
    {
        leds->setChannelPWM(i, 0);
    }

    for (int i = 0; i < 4; i++)
    {
        channels[i]->init();
    }
    
}