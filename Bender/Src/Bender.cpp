#include "Bender.h"

/**
 * @brief note that this function executes in the main task
 * 
 */
void Bender::init() {
    adc.setFilter(0.1);
    okSemaphore *sem = adc.initDenoising();
    sem->wait();
    currOutput = BENDER_DAC_ZERO; // initialize at idle position for filtering
    updateDAC(currOutput);
}

/**
 * @brief This function should read the current ADC value and then process it to 
 * determine the current bend position + map the ADC value so it ranges 
 * the full 16-bit range (for the DAC)
 *
 *
 */
void Bender::process()
{
    currBendPosition = adc.read_u16();
    
    // this switch block is a type of schmitt trigger based on the previous state of the bender
    switch (prevState)
    {
    case BENDING_IDLE:
        if (currBendPosition > adc.avgValueWhenIdle + BENDER_NOISE_THRESHOLD)
        {
            currState = BENDING_UP;
        }
        else if (currBendPosition < adc.avgValueWhenIdle - BENDER_NOISE_THRESHOLD)
        {
            currState = BENDING_DOWN;
        }
        break;
    case BENDING_UP:
        if (currBendPosition < adc.avgValueWhenIdle + BENDER_NOISE_THRESHOLD - BENDER_HYSTERESIS)
        {
            currState = BENDING_IDLE;
        }
        break;
    case BENDING_DOWN:
        if (currBendPosition > adc.avgValueWhenIdle - BENDER_NOISE_THRESHOLD + BENDER_HYSTERESIS)
        {
            currState = BENDING_IDLE;
        }
        break;
    }

    if (this->isIdle())
    {
        processedBend = BENDER_DAC_ZERO;

        if (idleCallback)
            idleCallback();
    }
    else
    {
        processedBend = calculateOutput(currBendPosition);

        if (activeCallback)
            activeCallback(processedBend);
    }

    // handle tri-state
    if (currState != prevState)
    {
        if (triStateCallback)
        {
            triStateCallback(currState);
        }
    }
    prevState = currState;
}

/**
 * @brief apply a slew filter and write the benders state to the DAC
 * NOTE: the conversion of ADC min/max to the full-range DAC output of 0..65535 should happen in this function
 * Output will be between 0V and 2.5V, centered at 2.5V/2
 */
void Bender::updateDAC(uint16_t value)
{
    prevOutput = currOutput;
    currOutput = filter_one_pole<uint16_t>(value, prevOutput, 0.065);
    dac->write(dac_chan, currOutput);
}

/**
 * Map the ADC input to a greater range so the DAC can make use of all 16-bits
 *
 * Two formulas are needed because the zero to max range and the min to zero range are usually different
 */
uint16_t Bender::calculateOutput(uint16_t value)
{
    uint16_t output;

    // BEND UP
    if (value > adc.avgValueWhenIdle && value < adc.inputMax)
    {
        output = (((float)dacOutputRange / (adc.inputMax - adc.avgValueWhenIdle)) * (value - adc.avgValueWhenIdle));
        return (BENDER_DAC_ZERO - output); // inverted
    }

    // BEND DOWN
    else if (value < adc.avgValueWhenIdle && value > adc.inputMin)
    {
        output = (((float)dacOutputRange / (adc.inputMin - adc.avgValueWhenIdle)) * (value - adc.avgValueWhenIdle));
        return (BENDER_DAC_ZERO + output); // non-inverted
    }
    // ELSE executes when a bender is poorly calibrated, and exceeds its max or min bend
    else
    {
        return currOutput; // return whatever the last calulated output was.
    }
}

bool Bender::isIdle()
{
    if (currState != BENDING_IDLE)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief determine the highest and lowest possible ADC readings when the bender component
 * is bent to its maximum position (in either direction)
 */
void Bender::calibrate()
{
    uint16_t sample = adc.read_u16();
    adc.sampleMinMax(sample);
}

uint16_t Bender::read()
{
    return adc.read_u16();
}

/**
 * @brief the average ADC value when bender is idle / not being touched
 *
 * @return uint16_t
 */
uint16_t Bender::getIdleValue()
{
    return adc.avgValueWhenIdle; // NOTE: careful with this value - you may prefer to use BENDER_DAC_ZERO instead
}

/**
 * @brief // the maximum value the ADC can achieve when Pitch Bend fully pressed
 *
 * @return uint16_t
 */
uint16_t Bender::getMaxBend()
{
    return adc.inputMax;
}

/**
 * @brief the minimum value the ADC can achieve when Pitch Bend fully pulled
 *
 * @return uint16_t
 */
uint16_t Bender::getMinBend()
{
    return adc.inputMin;
}

void Bender::attachIdleCallback(Callback<void()> func)
{
    idleCallback = func;
}

void Bender::attachActiveCallback(Callback<void(uint16_t bend)> func)
{
    activeCallback = func;
}

void Bender::attachTriStateCallback(Callback<void(BendState state)> func)
{
    triStateCallback = func;
}