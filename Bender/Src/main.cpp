#include "main.h"
#include "tasks.h"
#include "tim_api.h"
#include "logger.h"
#include "Callback.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "AnalogHandle.h"
#include "I2C.h"
#include "IS31FL3246.h"
#include "MPR121.h"
#include "DAC8554.h"
#include "Bender.h"

I2C i2c3(I2C3_SDA, I2C3_SCL, I2C::Instance::I2C_3);

MPR121 touch_pads(&i2c3, TOUCH_INT);
IS31FL3246 led_driver(&i2c3);
DAC8554 dac(SPI2_MOSI, SPI2_SCK, DAC_CS);

Bender benderA(0, &led_driver, &dac, DAC8554::Channel::CHAN_A, POT_ADC_A, BEND_ADC_A, CHAN_A_TRIG_LED_PIN);
Bender benderB(1, &led_driver, &dac, DAC8554::Channel::CHAN_B, POT_ADC_B, BEND_ADC_B, CHAN_B_TRIG_LED_PIN);
Bender benderC(2, &led_driver, &dac, DAC8554::Channel::CHAN_C, POT_ADC_C, BEND_ADC_C, CHAN_C_TRIG_LED_PIN);
Bender benderD(3, &led_driver, &dac, DAC8554::Channel::CHAN_D, POT_ADC_D, BEND_ADC_D, CHAN_D_TRIG_LED_PIN);

Controller controller(&led_driver, &benderA, &benderB, &benderC, &benderD);

void taskMain(void *pvParameters)
{
    i2c3.init();
    dac.init();
    touch_pads.init();

    controller.init();

    while (1)
    {
        for (int i = 0; i < 4; i++)
        {
            controller.channels[i]->process();
        }
        
        HAL_Delay(10);
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

    vTaskStartScheduler();

    while (1)
    {
    }
}
