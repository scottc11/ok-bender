#include "Controller.h"

int TIME_LED_PINS[6] = {14, 15, 16, 21, 22, 23};

void Controller::init()
{
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

    metro->init();
    
    metro->attachStepCallback(callback(this, &Controller::handleStepCallback));
    metro->attachResetCallback(callback(this, &Controller::handleClockCorrect));
    metro->attachBarResetCallback(callback(this, &Controller::handleBarReset));
    metro->attachPPQNCallback(callback(this, &Controller::handlePulse)); // always do this last
    metro->disableInputCaptureISR();

    HAL_Delay(1000); // post initialization
    rec_led.write(0);
    clear_led.write(0);
    reset_led.write(0);
    metro->start();
}

/**
 * @brief Called within ISR, advances all channels sequence by 1, and handles global clock output
 *
 * @param pulse
 */
void Controller::handlePulse(uint8_t pulse)
{
    if (pulse == 0)
    {
        rec_led.write(HIGH);
        if (metro->step == 0)
        {
            clear_led.write(HIGH);
        }
    }
    else if (pulse == 4)
    {
        rec_led.write(LOW);
        clear_led.write(LOW);
    }

    dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::ADVANCE, pulse);
}

/**
 * @brief reset all sequencers PPQN position to 0
 *
 * The issue might be that the sequence is not always running too slow, but running to fast. In other words, the
 * pulse count overtakes the external clocks rising edge. To avoid this scenario, you need to halt further execution of the sequence should it reach PPQN - 1
 * prior to the ext clock rising edge. Thing is, the sequence would first need to know that it is being externally clocked...
 *
 * Currently, your clock division is very off. The higher the ext clock signal is, you lose an increasing amount of pulses.
 * Ex. @ 120bpm sequence will reset on pulse 84 (ie. missing 12 PPQNs)
 * Ex. @ 132bpm sequence missing 20 PPQNs
 */
void Controller::handleClockCorrect(uint8_t pulse)
{
    dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::CORRECT, 0);
}

/**
 * @brief triggers in ISR everytime clock progresses by one step
 *
 * @param step
 */
void Controller::handleStepCallback(uint16_t step)
{
    dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::QUARTER_NOTE_OVERFLOW, 0);
}

/**
 * @brief function that triggers at the begining of every bar
 *
 */
void Controller::handleBarReset()
{
    dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::BAR_RESET, 0);
}