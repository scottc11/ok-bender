#include "main.h"
#include "tasks.h"
#include "tim_api.h"
#include "logger.h"
#include "Callback.h"
#include "DigitalOut.h"
#include "InterruptIn.h"
#include "AnalogHandle.h"
#include "MultiChanADC.h"
#include "I2C.h"
#include "IS31FL3246.h"
#include "MPR121.h"
#include "DAC8554.h"

I2C i2c3(I2C3_SDA, I2C3_SCL, I2C::Instance::I2C_3);

// MPR121 touchA(&i2c3, TOUCH_INT);

IS31FL3246 led_driver(&i2c3);

uint16_t AnalogHandle::DMA_BUFFER[ADC_DMA_BUFF_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};
PinName AnalogHandle::ADC_PINS[ADC_DMA_BUFF_SIZE] = {BEND_ADC_A, BEND_ADC_B, BEND_ADC_C, BEND_ADC_D, POT_ADC_A, POT_ADC_B, POT_ADC_C, POT_ADC_D };

DAC8554 dac(SPI2_MOSI, SPI2_SCK, DAC_CS);

AnalogHandle bendA(BEND_ADC_A);
AnalogHandle bendB(BEND_ADC_B);
AnalogHandle bendC(BEND_ADC_C);
AnalogHandle bendD(BEND_ADC_D);
AnalogHandle potA(POT_ADC_A);
AnalogHandle potB(POT_ADC_B);
AnalogHandle potC(POT_ADC_C);
AnalogHandle potD(POT_ADC_D);

Controller controller(&led_driver);

void taskMain(void *pvParameters)
{
    // i2c3.init();


    // led_driver.init();
    // for (int i = 1; i <= 32; i++)
    // {
    //     led_driver.setChannelPWM(i, 127);
    // }
    

    while (1)
    {
        // HAL_Delay(2000);
        potA.read_u16();
        for (int i = 0; i < 4; i++)
        {
                
        }
        // HAL_Delay(2000);
        for (int i = 0; i < 4; i++)
        {
            
        }
    }
}

int main(void)
{
    HAL_Init();
    
    SystemClock_Config();

    logger_init();
    logger_log("\nLogger Initialized\n");
    logger_log_system_config();

    multi_chan_adc_init();
    multi_chan_adc_start();

    HAL_Delay(100);

    xTaskCreate(taskMain, "taskMain", 512, NULL, 1, &th_main);
    xTaskCreate(task_interrupt_handler, "ISR handler", RTOS_STACK_SIZE_MIN, &controller, RTOS_PRIORITY_HIGH + 1, NULL);

    vTaskStartScheduler();

    while (1)
    {
    }
}
