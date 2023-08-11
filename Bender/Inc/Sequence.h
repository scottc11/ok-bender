#pragma once

#include "main.h"
#include "Bender.h"
#include "Callback.h"
#include "ArrayMethods.h"

#define SEQ_PROGRESS_MAX 15 // "15"
#define SEQ_PROGRESS_MIN 0  // "0"

// if you extend a base Sequencer class, you can use the same sequencer code for different types of sequences

class Sequence
{
public:
    Sequence()
    {
        setLength(DEFAULT_SEQ_LENGTH);
    };

    uint16_t events[MAX_SEQ_LENGTH_PPQN]; // raw ADC value from bender

    Callback<void()> recordOverflowCallback;

    int length;     // how many steps the sequence contains
    int lengthPPQN; // how many PPQN the sequence contains
    int maxLength;  // the maximum allowable steps in a sequence

    int currStepPosition; // the number of PPQN that have passed since the last step was advanced
    int currStep;         // current sequence step
    int prevStep;         // the previous step executed in the sequence
    int currPosition;     // current position of sequence (in PPQN)
    int prevPosition;
    int prevEventPos; // represents the position of the last event which got triggered (either HIGH or LOW)
    int newEventPos;  // when a new event is created, we store the position in this variable in case we need it for something (ie. sequence overdubing)

    int progressDiviser; // lengthPPQN / 16 (num LEDs used)
    int progress;

    bool adaptiveLength; // flag determining if the sequence length should increase past its current length
    bool overdub;        // flag gets set to true so that the sequence handler clears/overdubs existing events
    bool overwriteExistingEvents;
    
    bool playbackEnabled;     // when true, sequence will playback event list
    bool bendEnabled;         // flag used for overriding current recorded bend with active bend
    bool containsEvents;  // flag indicating if a sequence has any bend events

    static bool recordEnabled;  // when true, sequence will create and new events to the event list
    static bool recordArmed;    // when true, recording will be enabled the next time bar overflows
    static bool recordDisarmed; // when true, recording will be disabled the next time bar overflows

    void init();
    void reset();
    void resetStep();
    void createEvent(int position, uint16_t value);
    uint16_t getEvent(int position);
    void clearEvent(int position);
    void clearAllEvents();

    void copyPaste(int prevPosition, int newPosition);
    void cutPaste(int prevPosition, int newPosition);

    void setLength(int steps);
    void setMaxLength(int stepsPerBar);
    void setProgress();

    int getNextPosition(int position);
    int getPrevPosition(int position);
    int getLastPosition();

    void advance();

    void enableRecording(int stepsPerBar);
    void disableRecording();

    void enablePlayback();
    void disablePlayback();

    void enableOverdub();
    void disableOverdub();

    void attachRecordOverflowCallback(Callback<void()> func)
    {
        recordOverflowCallback = func;
    }
};