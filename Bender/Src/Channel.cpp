#include "Channel.h"

void Channel::init() {
    pot_adc.setFilter(0.1);
    bender->init();
}

void Channel::setTrigLed(int pwm)
{
    led_driver->setChannelPWM(trig_led_pin, pwm);
}

void Channel::handlePulse(int pulse)
{    
    if (sequence.playbackEnabled)
    {
        // Handle Events (only if the bender is currently idle / inactive)
        if (sequence.containsEvents)
        {
            if (bender->isIdle())
            {
                bender->updateDAC(sequence.getEvent(sequence.currPosition));
            } else {
                bender->process();
            }
        }
    } else {
        bender->process();
    }
    sequence.advance();
}