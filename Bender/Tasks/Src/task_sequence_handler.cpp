#include "task_sequence_handler.h"

TaskHandle_t sequencer_task_handle;
QueueHandle_t sequencer_queue;

/**
 * @brief Task which listens for a notification from the SuperClock
 *
 * @param params
 */
void task_sequence_handler(void *params)
{
    Controller *ctrl = (Controller *)params;
    sequencer_task_handle = xTaskGetCurrentTaskHandle();
    sequencer_queue = xQueueCreate(96, sizeof(uint32_t));
    uint32_t event = 0x0;
    bool resetArmed = false;

    while (1)
    {
        // queue == [advance, advance, advance, clear, advance, freeze, advance, advance ]
        xQueueReceive(sequencer_queue, &event, portMAX_DELAY);
        CHAN channel = (CHAN)bitwise_slice(event, 24, 8);
        SEQ action = (SEQ)bitwise_slice(event, 16, 8);
        uint16_t data = bitwise_slice(event, 0, 16);

        switch (action)
        {
        case SEQ::ADVANCE:
            // if (SuperSeq::recordArmed || SuperSeq::recordDisarmed)
            // {
            //     if (data % 24 == 0)
            //     { // data in this case is the current pulse
            //         ctrl->recLED.toggle();
            //     }
            // }
            for (int i = 0; i < 4; i++)
            {
                ctrl->channels[i]->handlePulse(data);
            }
            
            break;

        case SEQ::HANDLE_SELECT_PAD:
            break;

        case SEQ::FREEZE:
            break;

        case SEQ::RESET_ARM:
            break;

        // time this so that it triggers:
        //  if pressed 12 PPQN before beat 1, wait to reset once beat 1 happens
        //  if < 12 ppqn after beat 1, reset immediately
        case SEQ::RESET:
            ctrl->metro->reset();
            for (int i = 0; i < 4; i++)
            {
                ctrl->channels[channel]->sequence.reset();
            }
            break;

        case SEQ::CLEAR_BEND:
            // if (channel == CHAN::ALL)
            // {
            //     for (int i = 0; i < CHANNEL_COUNT; i++)
            //     {
            //         ctrl->channels[i]->sequence.clearAllBendEvents();
            //     }
            // }
            // else
            // {
            //     ctrl->channels[channel]->sequence.clearAllBendEvents();
            // }
            break;

        // when the bar overflows back to step 1 (ex. step 4 to step 1)
        case SEQ::BAR_RESET:
            // if (SuperSeq::recordArmed)
            // {
            //     SuperSeq::recordArmed = false;
            //     dispatch_sequencer_event(CHAN::ALL, SEQ::RECORD_ENABLE, 0);
            // }
            // else if (SuperSeq::recordDisarmed)
            // {
            //     SuperSeq::recordDisarmed = false;
            //     dispatch_sequencer_event(CHAN::ALL, SEQ::RECORD_DISABLE, 0);
            // }
            break;

        // every qaurternote
        case SEQ::QUARTER_NOTE_OVERFLOW:
            if (resetArmed)
            {
                resetArmed = false;
                dispatch_sequencer_event(channel, SEQ::RESET, 0);
            }
            break;

        case SEQ::RECORD_ENABLE:
            // ctrl->recLED.write(1);
            // for (int i = 0; i < CHANNEL_COUNT; i++)
            //     ctrl->channels[i]->enableSequenceRecording();
            break;

        case SEQ::RECORD_DISABLE:
            // ctrl->recLED.write(0);
            // for (int i = 0; i < CHANNEL_COUNT; i++)
            //     ctrl->channels[i]->disableSequenceRecording();
            break;

        case SEQ::RECORD_ARM:
            // SuperSeq::recordDisarmed = false;
            // SuperSeq::recordArmed = true;
            break;

        case SEQ::RECORD_DISARM:
            // SuperSeq::recordArmed = false;
            // SuperSeq::recordDisarmed = true;
            break;

        case SEQ::RECORD_OVERFLOW:
            // if (ctrl->channels[channel]->sequence.containsEvents())
            //     ctrl->channels[channel]->drawSequenceToDisplay(true);
            break;

        case SEQ::INCREMENT_TIME_SIG:
            // ctrl->clock->setStepsPerBar(ctrl->clock->stepsPerBar + 1);
            // ctrl->drawTimeSignatureToDisplay();
            break;

        case SEQ::DECREMENT_TIME_SIG:
            // ctrl->clock->setStepsPerBar(ctrl->clock->stepsPerBar - 1);
            // ctrl->drawTimeSignatureToDisplay();
            break;

        case SEQ::CORRECT:
            // for (int i = 0; i < CHANNEL_COUNT; i++)
            // {
            //     // you could just setting the sequence to 0, set any potential gates low. You may miss a note but 🤷‍♂️

            //     // if sequence is not on its final PPQN of its step, then trigger all remaining PPQNs in current step until currPPQN == 0
            //     if (ctrl->channels[i]->sequence.currStepPosition != 0)
            //     {
            //         while (ctrl->channels[i]->sequence.currStepPosition != 0)
            //         {
            //             // incrementing the clock will at least keep the sequence in sync with an external clock
            //             ctrl->channels[i]->sequence.advance();
            //             ctrl->channels[i]->handleClock();
            //         }
            //     }
            // }
            break;
        
        case SEQ::ENTER_CALIBRATION_MODE:
            ctrl->metro->stop();
            ctrl->calibrating = true;
            for (int i = 0; i < 4; i++)
            {
                ctrl->channels[i]->bender->adc.resetMinMax();
            }
            ctrl->rec_led.write(1);
            ctrl->clear_led.write(1);
            ctrl->reset_led.write(1);
            break;
        
        case SEQ::EXIT_CALIBRATION_MODE:
            ctrl->rec_led.write(0);
            ctrl->clear_led.write(0);
            ctrl->reset_led.write(0);
            ctrl->saveCalibrationDataToFlash();
            ctrl->metro->start();
            ctrl->calibrating = false;
            break;
        }
    }
}

void dispatch_sequencer_event(CHAN channel, SEQ action, uint16_t position)
{
    // | chan | event | position |
    uint32_t event = ((uint8_t)channel << 24) | ((uint8_t)action << 16) | position;
    xQueueSend(sequencer_queue, &event, portMAX_DELAY);
}

void dispatch_sequencer_event_ISR(CHAN channel, SEQ action, uint16_t position)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // | chan | event | position |
    uint32_t event = ((uint8_t)channel << 24) | ((uint8_t)action << 16) | position;
    xQueueSendFromISR(sequencer_queue, &event, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void suspend_sequencer_task()
{
    vTaskSuspend(sequencer_task_handle);
}

void resume_sequencer_task()
{
    vTaskResume(sequencer_task_handle);
}