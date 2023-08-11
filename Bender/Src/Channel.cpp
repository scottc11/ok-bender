#include "Channel.h"

void Channel::init() {
    pot_adc.setFilter(0.1);
    bender->init();
    bender->attachActiveCallback(callback(this, &Channel::benderActiveCallback));
    bender->attachIdleCallback(callback(this, &Channel::benderIdleCallback));
    sequence.init();
}

void Channel::setTrigLed(int pwm)
{
    led_driver->setChannelPWM(trig_led_pin, pwm);
}

void Channel::handlePulse(int pulse)
{
    bender->process();
    
    if (sequence.playbackEnabled)
    {
        // Handle Events (only if the bender is currently idle / inactive)
        if (sequence.containsEvents)
        {
            if (bender->isIdle())
            {
                bender->updateDAC(sequence.getEvent(sequence.currPosition));
            }
        }
    }

    sequence.advance();
}

/**
 * @brief callback that executes when a bender is not in its idle state
 *
 * @param value the raw ADC value from the bender instance
 */
void Channel::benderActiveCallback(uint16_t value)
{
    if (!bender->override)
    {
        // override existng bend events when record disabled (but sequencer still ON)
        bender->updateDAC(value);

        // overdub existing bend events when record enabled
        if (sequence.recordEnabled)
        {
            sequence.createEvent(sequence.currPosition, value);
        }
    }
}

void Channel::benderIdleCallback()
{
    if (sequence.playbackEnabled)
    {
        if (!sequence.containsEvents)
        {
            bender->updateDAC(bender->processedBend); // currBend gets set to ist idle value in the underlying handler
        }
    }
    else
    {
        bender->updateDAC(bender->processedBend);
    }
}