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
#include "MCP23017.h"
#include "SX1509.h"
#include "DAC8554.h"
#include "VoltPerOctave.h"
#include "SuperClock.h"
#include "Degrees.h"
#include "Controller.h"
#include "TouchChannel.h"

I2C i2c1(I2C1_SDA, I2C1_SCL, I2C::Instance::I2C_1);
I2C i2c3(I2C3_SDA, I2C3_SCL, I2C::Instance::I2C_3);

MPR121 touchA(&i2c1, TOUCH_INT_A);
MPR121 touchB(&i2c1, TOUCH_INT_B, MPR121::ADDR_VDD);
MPR121 touchC(&i2c1, TOUCH_INT_C, MPR121::ADDR_SCL);
MPR121 touchD(&i2c1, TOUCH_INT_D, MPR121::ADDR_SDA);
MPR121 touchE(&i2c3, TOUCH_INT_E);

SX1509 ledsA(&i2c3, SX1509_CHAN_A_ADDR);
SX1509 ledsB(&i2c3, SX1509_CHAN_B_ADDR);
SX1509 ledsC(&i2c3, SX1509_CHAN_C_ADDR);
SX1509 ledsD(&i2c3, SX1509_CHAN_D_ADDR);

IS31FL3246 led_driver(&i2c3);
IS31FL3246 led_driver2(&i2c3, IS31FL3246_ADDR_VCC);

SuperClock clock;

MCP23017 toggleSwitches(&i2c3, MCP23017_DEGREES_ADDR);
MCP23017 buttons(&i2c3, MCP23017_CTRL_ADDR);

uint16_t AnalogHandle::DMA_BUFFER[ADC_DMA_BUFF_SIZE] = {0, 0, 0, 0};
PinName AnalogHandle::ADC_PINS[ADC_DMA_BUFF_SIZE] = {ADC_A, ADC_B, ADC_C, ADC_D};

DAC8554 dac(SPI2_MOSI, SPI2_SCK, DAC_CS);
AnalogHandle adc(ADC_A);

VoltPerOctave output(&dac, DAC8554::Channel::CHAN_A, &adc);

Degrees degrees(DEGREES_INT, &toggleSwitches);

TouchChannel chanA(0, &clock, &touchA, &ledsA, &degrees, &dac, DAC8554::CHAN_A, ADC_A, GATE_OUT_A);
TouchChannel chanB(1, &clock, &touchB, &ledsB, &degrees, &dac, DAC8554::CHAN_B, ADC_B, GATE_OUT_B);
TouchChannel chanC(2, &clock, &touchC, &ledsC, &degrees, &dac, DAC8554::CHAN_C, ADC_C, GATE_OUT_C);
TouchChannel chanD(3, &clock, &touchD, &ledsD, &degrees, &dac, DAC8554::CHAN_D, ADC_D, GATE_OUT_D);

Controller controller(&clock, &chanA, &chanB, &chanC, &chanD, &degrees, &buttons, &led_driver);

void taskMain(void *pvParameters)
{
    i2c1.init();
    i2c3.init();

    controller.init();

    led_driver2.init();
    led_driver2.setChannelPWM(6, 20);
    led_driver2.setChannelPWM(7, 20);
    led_driver2.setChannelPWM(8, 20);
    led_driver2.setChannelPWM(9, 20);

    controller.recLED.write(1);
    controller.freezeLED.write(1);
    controller.tempoLED.write(1);

    while (1)
    {
        HAL_Delay(2000);
        for (int i = 0; i < 4; i++)
        {
            controller.channels[i]->output.setPitch(0);
        }
        HAL_Delay(2000);
        for (int i = 0; i < 4; i++)
        {
            controller.channels[i]->output.setPitch(11);
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
    xTaskCreate(task_sequence_handler, "sequencer", RTOS_STACK_SIZE_MAX, &controller, RTOS_PRIORITY_HIGH - 2, NULL);

    vTaskStartScheduler();

    while (1)
    {
    }
}
