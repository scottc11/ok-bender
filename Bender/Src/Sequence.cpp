#include "Sequence.h"

bool Sequence::recordEnabled = false;
bool Sequence::recordArmed = false;
bool Sequence::recordDisarmed = false;

void Sequence::init()
{
    this->clearAllEvents();
};

/**
 * @brief will return position + 1 as long as it isn't the last pulse in the sequence
 */
int Sequence::getNextPosition(int position)
{
    if (position + 1 < lengthPPQN)
    {
        return position + 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief get the position of the given position - 1
 *
 * @param position
 * @return int
 */
int Sequence::getPrevPosition(int position)
{
    if (position == 0)
    {
        return lengthPPQN - 1;
    }
    else
    {
        return position - 1;
    }
}

int Sequence::getLastPosition()
{
    return lengthPPQN - 1;
}

/**
 * @brief advance the sequencer position by 1
 * @note you cant make any I2C calls in these functions, you must defer them to a seperate thread to be executed later
 */
void Sequence::advance()
{
    prevPosition = currPosition;
    currPosition += 1;
    currStepPosition += 1;

    if (currPosition % PPQN == 0)
    {
        prevStep = currStep;
        currStep += 1;
        currStepPosition = 0;
    }

    if (currPosition > lengthPPQN - 1)
    {
        if (recordEnabled && adaptiveLength)
        {
            // disable adaptive length, set seq length to max, and enable overdub
            if (currStep >= maxLength)
            {
                adaptiveLength = false;
                this->setLength(maxLength);
                currPosition = 0;
                currStep = 0;
                overwriteExistingEvents = true; // enable overdub
                if (recordOverflowCallback)
                    recordOverflowCallback();
            }
            else
            {
                // do nothing, let things keep counting upwards
            }
        }
        else // reset loop
        {
            currPosition = 0;
            currStep = 0;
        }
    }
}

void Sequence::enableRecording(int stepsPerBar /*time signature*/)
{
    this->recordEnabled = true;
    // if no currently recorded events, enable adaptive length
    if (!this->containsEvents)
    {
        this->reset();
        this->setLength(2);
        this->setMaxLength(stepsPerBar);
        this->adaptiveLength = true;
    }
    else
    {
        this->overwriteExistingEvents = true;
    }
}

void Sequence::disableRecording()
{
    this->recordEnabled = false;
    this->overwriteExistingEvents = false;
    if (this->containsEvents && adaptiveLength)
    {
        this->adaptiveLength = false;
        this->setLength(this->currStep);
    }
}

void Sequence::enablePlayback()
{
    playbackEnabled = true;
}

void Sequence::disablePlayback()
{
    playbackEnabled = false;
}

void Sequence::enableOverdub()
{
    overdub = true;
}

void Sequence::disableOverdub()
{
    overdub = false;
}

/**
 * @brief reset the sequence
 */
void Sequence::reset()
{
    prevPosition = currPosition;
    currPosition = 0;
    prevStep = currStep;
    currStep = 0;
};

/**
 * @brief reset the current position to the position equal to the last step
 */
void Sequence::resetStep()
{
    currPosition = (currPosition - (currPosition % (PPQN - 1))) - (PPQN - 1);
}

/**
 * @brief Deactivate all events in event array and set empty flag
 */
void Sequence::clearAllEvents()
{
    for (int i = 0; i < PPQN * MAX_SEQ_LENGTH; i++)
    {
        clearEvent(i);
    }
    playbackEnabled = false;
    containsEvents = false;
};

/**
 * @brief copy the contents of an event from one position to a new position
 *
 * @param prevPosition
 * @param newPosition
 */
void Sequence::copyPaste(int prevPosition, int newPosition)
{
    events[newPosition] = events[prevPosition];
}

/**
 * @brief copy the contents of an event from one position to a new position
 *
 * @param prevPosition
 * @param newPosition
 */
void Sequence::cutPaste(int prevPosition, int newPosition)
{
    this->copyPaste(prevPosition, newPosition);
    clearEvent(prevPosition);
}


/**
 * @brief Set how many steps a sequence will contain (not PPQN)
 */
void Sequence::setLength(int steps)
{
    if (steps > 0 && steps <= MAX_SEQ_LENGTH)
    {
        length = steps;
        lengthPPQN = length * PPQN;
        progressDiviser = lengthPPQN / SEQ_PROGRESS_MAX;
    }
};

void Sequence::setMaxLength(int stepsPerBar)
{
    maxLength = (MAX_SEQ_LENGTH / stepsPerBar) * stepsPerBar;
    if (maxLength > MAX_SEQ_LENGTH)
    {
        maxLength = MAX_SEQ_LENGTH;
    }
}

void Sequence::setProgress()
{
    this->progress = this->currPosition / this->progressDiviser;
}

// POSSIBLE CLASS INHERITANCE FUNCTIONS

void Sequence::createEvent(int position, uint16_t value)
{
    events[position] = value;
    containsEvents = true;
}

uint16_t Sequence::getEvent(int position)
{
    return events[position];
}

void Sequence::clearEvent(int position)
{
    events[position] = BENDER_DAC_ZERO;
}