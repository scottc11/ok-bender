#include "Channel.h"

void Channel::init() {
    bender->init();
    bender->attachActiveCallback(callback(this, &Channel::benderActiveCallback));
    bender->attachIdleCallback(callback(this, &Channel::benderIdleCallback));
    sequence.init();
    ratchet.init(BENDER_DAC_ZERO, 0, BIT_MAX_16);
    ratchet.attachCallback(callback(this, &Channel::ratchetHandler));
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
                uint16_t bend = sequence.getEvent(sequence.currPosition);
                bender->updateDAC(bend);
                ratchet.handleRatchet(sequence.currPosition, bend);
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
        ratchet.handleRatchet(sequence.currPosition, value);
        // overdub existing bend events when record enabled
        if (sequence.recordEnabled)
        {
            sequence.createEvent(sequence.currPosition, value);
        }
    }
}

/**
 * @brief callback that executes when a bender is in its idle state
 * 
 */
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

/**
 * @brief callback that executes when the ratchet state changes
 * 
 * @param state 
 */
void Channel::ratchetHandler(bool state) {
    if (state) {
        trig_out.write(0); // inverted output!
        setTrigLed(100);
    } else {
        trig_out.write(1);
        setTrigLed(0);
    }
}