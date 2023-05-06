#include "task_interrupt_handler.h"

/**
 * @brief A task which listens to a queue of interrupt events
 *
 * @param params global control
 */
void task_interrupt_handler(void *params)
{
    Controller *controller = (Controller *)params;
    thInterruptHandler = xTaskGetCurrentTaskHandle();
    qhInterruptQueue = xQueueCreate(64, sizeof(uint8_t));
    logger_log_task_watermark();
    uint8_t isr_source;
    while (1)
    {
        xQueueReceive(qhInterruptQueue, &isr_source, portMAX_DELAY);
        switch (isr_source)
        {
        case ISR_ID_TOGGLE_SWITCHES:
            logger_log("\n### Toggle Switch ISR ###\n");
            controller->switches->updateDegreeStates();
            break;
        case ISR_ID_TACTILE_BUTTONS:
            logger_log("\n### Tactile Buttons ISR ###\n");
            controller->pollButtons();
            break;
        case ISR_ID_TOUCH_PADS:
            logger_log("\n### Touch Pads ISR ###\n");
            // controller->pollTouchPads();
            break;
        default:
            break;
        }
    }
}