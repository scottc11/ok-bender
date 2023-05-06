#pragma once

#include "main.h"
#include "task_interrupt_handler.h"
#include "task_sequence_handler.h"

extern TaskHandle_t th_main;
extern TaskHandle_t thInterruptHandler;
extern QueueHandle_t qhInterruptQueue;
