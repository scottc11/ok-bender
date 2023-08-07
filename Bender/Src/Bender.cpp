#include "Bender.h"


void Bender::init() {
    pot_adc.setFilter(0.1);
    bend_adc.setFilter(0.1);

    setTrigLed(0);
}

void Bender::process() {
    prevOutput = currOutput;
    currOutput = filter_one_pole<uint16_t>(pot_adc.read_u16(), prevOutput, 0.065);
    dac->write(dac_chan, currOutput);
}

void Bender::setTrigLed(int pwm)
{
    leds->setChannelPWM(trig_led_pin, pwm);
}