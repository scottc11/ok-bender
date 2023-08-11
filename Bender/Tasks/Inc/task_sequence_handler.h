#pragma once

#include "main.h"
#include "logger.h"
#include "Controller.h"

extern TaskHandle_t sequencer_task_handle;
extern QueueHandle_t sequencer_queue;

enum class SEQ
{
    ADVANCE,
    FREEZE,
    RESET_ARM,
    RESET,
    BAR_RESET,
    QUARTER_NOTE_OVERFLOW,
    CLEAR,
    RECORD_ENABLE,
    RECORD_DISABLE,
    RECORD_ARM,
    RECORD_DISARM,
    RECORD_OVERFLOW,
    SET_LENGTH,
    INCREMENT_TIME_SIG,
    DECREMENT_TIME_SIG,
    CORRECT,
    HANDLE_SELECT_PAD,
    ENTER_CALIBRATION_MODE,
    EXIT_CALIBRATION_MODE
};
typedef enum SEQ SEQ;

void task_sequence_handler(void *params);
void dispatch_sequencer_event(CHAN channel, SEQ event, uint16_t position);
void dispatch_sequencer_event_ISR(CHAN channel, SEQ event, uint16_t position);
void suspend_sequencer_task();
void resume_sequencer_task();