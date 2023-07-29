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

AnalogHandle potA(POT_ADC_A);
AnalogHandle potB(POT_ADC_B);
AnalogHandle potC(POT_ADC_C);
AnalogHandle potD(POT_ADC_D);
DigitalOut rec_led(REC_LED);

I2C i2c3(I2C3_SDA, I2C3_SCL, I2C::Instance::I2C_3);

MPR121 touchA(&i2c3, TOUCH_INT);
IS31FL3246 led_driver(&i2c3);
DAC8554 dac(SPI2_MOSI, SPI2_SCK, DAC_CS);

Controller controller(&led_driver);

void taskMain(void *pvParameters)
{
    i2c3.init();
    dac.init();

    touchA.init();

    led_driver.init();
    led_driver.setGlobalCurrent(100, 100, 100);
    led_driver.setControlRegister(false, false, IS31FL3246::PWM_Frequency::_64kHz, false);
    for (int i = 1; i <= 36; i++)
    {
        led_driver.setChannelPWM(i, 127);
    }

    uint16_t counter = 0;

    while (1)
    {
        rec_led.toggle();
        dac.write(DAC8554::Channel::CHAN_A, potA.read_u16());
        dac.write(DAC8554::Channel::CHAN_B, potB.read_u16());
        dac.write(DAC8554::Channel::CHAN_C, potC.read_u16());
        dac.write(DAC8554::Channel::CHAN_D, potD.read_u16());
        counter += 1000;
        HAL_Delay(100);
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
