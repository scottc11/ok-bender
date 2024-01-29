#include "main.h"
#include "tasks.h"
#include "tim_api.h"
#include "logger.h"
#include "Callback.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "AnalogHandle.h"
#include "Metronome.h"
#include "I2C.h"
#include "IS31FL3246.h"
#include "MPR121.h"
#include "DAC8554.h"
#include "Channel.h"
#include "Bender.h"
#include "task_sequence_handler.h"

I2C i2c3(I2C3_SDA, I2C3_SCL, I2C::Instance::I2C_3);

MPR121 touch_pads(&i2c3, TOUCH_INT);
IS31FL3246 led_driver(&i2c3);
DAC8554 dac(SPI2_MOSI, SPI2_SCK, DAC_CS);

Metronome metronome;

Bender benderA(&dac, DAC8554::Channel::CHAN_A, BEND_ADC_A, POT_ADC_A);
Bender benderB(&dac, DAC8554::Channel::CHAN_B, BEND_ADC_B, POT_ADC_B);
Bender benderC(&dac, DAC8554::Channel::CHAN_C, BEND_ADC_C, POT_ADC_C);
Bender benderD(&dac, DAC8554::Channel::CHAN_D, BEND_ADC_D, POT_ADC_D);

Channel chanA(0, &led_driver, CHAN_A_TRIG_LED_PIN, TRIG_OUT_A, &benderA);
Channel chanB(1, &led_driver, CHAN_B_TRIG_LED_PIN, TRIG_OUT_B, &benderB);
Channel chanC(2, &led_driver, CHAN_C_TRIG_LED_PIN, TRIG_OUT_C, &benderC);
Channel chanD(3, &led_driver, CHAN_D_TRIG_LED_PIN, TRIG_OUT_D, &benderD);

Controller controller(&touch_pads, &led_driver, &metronome, &chanA, &chanB, &chanC, &chanD);

void taskMain(void *pvParameters)
{
    i2c3.init();
    dac.init();
    controller.init();

    while (1)
    {
        if (controller.calibrating) {
            for (int i = 0; i < 4; i++)
            {
                controller.channels[i]->bender->calibrate();
            }
        }
    }
}

int main(void)
{
    HAL_Init();
    
    SystemClock_Config();

    AnalogHandle::initialize();
    HAL_Delay(100);

    xTaskCreate(taskMain, "taskMain", 512, NULL, 1, &th_main);
    xTaskCreate(task_interrupt_handler, "ISR handler", RTOS_STACK_SIZE_MIN, &controller, RTOS_PRIORITY_HIGH + 1, NULL);
    xTaskCreate(task_sequence_handler, "sequencer", RTOS_STACK_SIZE_MAX / 4, &controller, RTOS_PRIORITY_HIGH, NULL);

    vTaskStartScheduler();

    while (1)
    {
    }
}
