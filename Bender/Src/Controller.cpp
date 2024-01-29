#include "Controller.h"

void Controller::init()
{
    loadCalibrationDataFromFlash();

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

    touch_pads->init();
    touch_pads->attachInterruptCallback(callback(this, &Controller::handleTouchInterrupt));
    touch_pads->attachCallbackTouched(callback(this, &Controller::onTouch));
    touch_pads->attachCallbackReleased(callback(this, &Controller::onRelease));
    touch_pads->enable();

    tp_reset.rise(callback(this, &Controller::handleClockReset));

    rec_btn.fall(callback(this, &Controller::handleRecBtn));
    clear_btn.fall(callback(this, &Controller::handleClearBtn));
    reset_btn.fall(callback(this, &Controller::handleResetBtn));
    ts_btn.fall(callback(this, &Controller::handleTsBtn));

    metro->init();
    metro->attachStepCallback(callback(this, &Controller::handleStepCallback));
    metro->attachResetCallback(callback(this, &Controller::handleClockCorrect));
    metro->attachBarResetCallback(callback(this, &Controller::handleBarReset));
    metro->attachPPQNCallback(callback(this, &Controller::handlePulse)); // always do this last
    metro->enableInputCaptureISR(); // comment this line out when testing without external clock

    HAL_Delay(100); // post initialization
    rec_led.write(0);
    clear_led.write(0);
    reset_led.write(0);
    
    this->setTimeSignature(3); // initialize at 4/4
    metro->start();
}

void Controller::onTouch(uint8_t pad)
{
    // map incoming pad to one of the 4 channels
    int channel = TOUCH_PAD_MAP[pad];
    if (channel < 4) {
        // if (channels[channel]->sequence.recordEnabled)
        // {
        // }
        channels[channel]->ratchetHandler(true);
    }
}

void Controller::onRelease(uint8_t pad)
{
    int channel = TOUCH_PAD_MAP[pad];
    if (channel < 4)
    {
        channels[channel]->ratchetHandler(false);
    }
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
        reset_led.write(HIGH);
        if (metro->step == 0)
        {
            clear_led.write(HIGH);
        }
    }
    else if (pulse == 4)
    {
        reset_led.write(LOW);
        clear_led.write(LOW);
    }

    dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::ADVANCE, pulse);
}

void Controller::handleClockReset()
{
    dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::RESET, 0);
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

void Controller::handleRecBtn() {
    if (Sequence::recordArmed || Sequence::recordEnabled)
    {
        dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::RECORD_DISARM, 0);
    }
    else if (Sequence::recordDisarmed || !Sequence::recordEnabled)
    {
        dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::RECORD_ARM, 0);
    }
}

void Controller::handleClearBtn() {
    dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::CLEAR, 0);
}

void Controller::handleResetBtn() {
    if (ts_btn.read() == LOW) {
        if (!calibrating) {
            dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::ENTER_CALIBRATION_MODE, 0);
        } else {
            dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::EXIT_CALIBRATION_MODE, 0);
        }
    } else {
        dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::RESET, 0);
    }
}

void Controller::handleTsBtn()
{
    dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::INCREMENT_TIME_SIG, 0);
}

void Controller::incrementTimeSignature()
{
    if (timeSigIndex < NUM_TIME_SIG_OPTIONS - 1)
    {
        timeSigIndex++;
    }
    else
    {
        timeSigIndex = 0;
    }
    setTimeSignature(timeSigIndex);
}

void Controller::setTimeSignature(int index)
{
    if (index < 0 || index >= NUM_TIME_SIG_OPTIONS)
        return;
    for (int i = 0; i < NUM_TIME_SIG_OPTIONS; i++)
    {
        leds->setChannelPWM(TIME_SIG_LEDS[i], 0);
    }
    timeSigIndex = index;
    leds->setChannelPWM(TIME_SIG_LEDS[timeSigIndex], 20);
    metro->setStepsPerBar(TIME_SIG_VALUE_MAP[timeSigIndex]);
}

// NOTE: the current hardware (for V2 I believe) doesn't have the interrupt pin connected (so this won't get called)
void Controller::handleTouchInterrupt()
{
    dispatch_sequencer_event_ISR(CHAN::ALL, SEQ::HANDLE_SELECT_PAD, 0);
}

/**
 * @brief Copy each channels config data into flash
*/
void Controller::saveCalibrationDataToFlash() {
    Flash flash;
    flash.erase(FLASH_CALIBRATION_ADDR);
    uint32_t data_buffer[2];
    for (int chan = 0; chan < CHANNEL_COUNT; chan++) // channel iterrator
    {
        uint32_t address_offset = FLASH_CALIBRATION_BLOCK_SIZE * chan;

        // max and min Bender calibration data
        data_buffer[0] = channels[chan]->bender->adc.getInputMin();
        data_buffer[1] = channels[chan]->bender->adc.getInputMax();
        flash.write(FLASH_CALIBRATION_ADDR + address_offset, data_buffer, 2);
    }
}

/**
 * @brief load calibration data from flash storage
 * 
 */
void Controller::loadCalibrationDataFromFlash()
{
    uint32_t data_buffer[4];
    Flash flash;
    flash.read(FLASH_CALIBRATION_ADDR, data_buffer, 4);

    bool configDataEmpty = flash.validate(data_buffer, 4);

    // if no config data or firmware version mismatch, load default configuration
    if (configDataEmpty)
    {
        logger_log("\nCalibration data source: DEFAULT");
    }
    else // load the data from flash
    {
        logger_log("\nChannel Settings Source: FLASH");
        for (int chan = 0; chan < CHANNEL_COUNT; chan++)
        {
            uint32_t address_offset = FLASH_CALIBRATION_BLOCK_SIZE * chan;
            flash.read(FLASH_CALIBRATION_ADDR + address_offset, data_buffer, 2);
            channels[chan]->bender->setMinBend((uint16_t)data_buffer[0]);
            channels[chan]->bender->setMaxBend((uint16_t)data_buffer[1]);
        }
    }
}