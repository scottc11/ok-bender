#pragma once

#include "main.h"
#include "logger.h"
#include "Controller.h"
#include "SuperSeq.h"

enum class SEQ
{
    ADVANCE,
    FREEZE,
    RESET_ARM,
    RESET,
    BAR_RESET,
    QUARTER_NOTE_OVERFLOW,
    CLEAR_TOUCH,
    CLEAR_BEND,
    RECORD_ENABLE,
    RECORD_DISABLE,
    RECORD_ARM,
    RECORD_DISARM,
    RECORD_OVERFLOW,
    TOGGLE_MODE,
    SET_LENGTH,
    INCREMENT_TIME_SIG,
    DECREMENT_TIME_SIG,
    QUANTIZE,
    CORRECT,
    HANDLE_TOUCH,
    HANDLE_DEGREE,
    DISPLAY
};
typedef enum SEQ SEQ;

enum CHAN
{
    A,
    B,
    C,
    D,
    ALL
};
typedef enum CHAN CHAN;

void task_sequence_handler(void *params);
void dispatch_sequencer_event(CHAN channel, SEQ event, uint16_t position);
void dispatch_sequencer_event_ISR(CHAN channel, SEQ event, uint16_t position);
void suspend_sequencer_task();
void resume_sequencer_task();