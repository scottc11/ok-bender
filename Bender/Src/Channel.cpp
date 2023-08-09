#include "Channel.h"

void Channel::init() {
    pot_adc.setFilter(0.1);
    bender->init();
}

void Channel::setTrigLed(int pwm)
{
    led_driver->setChannelPWM(trig_led_pin, pwm);
}