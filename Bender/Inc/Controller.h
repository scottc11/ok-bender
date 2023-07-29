#pragma once

#include "main.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "IS31FL3246.h"

class Controller
{
public:
    Controller(
        IS31FL3246 *leds_ptr) : touchInterrupt(TOUCH_INT, PullUp)
    {
        leds = leds_ptr;
    }
    IS31FL3246 *leds;
    InterruptIn touchInterrupt; // interupt pin for MPR121

    void __attribute__((optimize("O0"))) init();
};