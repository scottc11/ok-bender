#pragma once

#include "cmsis_os.h"
#include "common.h"

#ifndef FIRMWARE_VERSION // this will be passed in as a flag by compiler
#define FIRMWARE_VERSION "default"
#endif

#define CHANNEL_COUNT 4

#define PPQN 96
#define PPQN_DIV_2 (PPQN / 2)
#define PPQN_DIV_4 (PPQN / 4)
#define PPQN_DIV_8 (PPQN / 8)

#define DEFAULT_SEQ_LENGTH 8
#define MAX_SEQ_LENGTH 32
#define MAX_SEQ_LENGTH_PPQN (MAX_SEQ_LENGTH * PPQN)

#define FLASH_CALIBRATION_ADDR ADDR_FLASH_SECTOR_6
#define FLASH_CALIBRATION_BLOCK_SIZE 32 // 8 4-byte words

#define EXT_CLOCK_INPUT PA_3
#define TP_RESET PA_1

#define I2C3_SDA PC_9
#define I2C3_SCL PA_8

#define DAC_CS PB_12
#define SPI2_MOSI PB_15
#define SPI2_MISO PB_14
#define SPI2_SCK PB_13

#define ADC_DMA_BUFFER_SIZE 8

#define BEND_ADC_A PB_1
#define BEND_ADC_B PC_5
#define BEND_ADC_C PA_7
#define BEND_ADC_D PA_5

#define POT_ADC_A PB_0
#define POT_ADC_B PC_4
#define POT_ADC_C PA_6
#define POT_ADC_D PA_4

#define TRIG_OUT_A PB_4
#define TRIG_OUT_B PB_5
#define TRIG_OUT_C PB_6
#define TRIG_OUT_D PB_7

#define REC_LED PC_2
#define CLEAR_LED PC_0
#define RESET_LED PC_7

#define TIME_SIG_BTN PC_13
#define REC_BTN PC_6
#define RESET_BTN PA_9
#define CLEAR_BTN PA_0

#define CHAN_A_TRIG_LED_PIN 11
#define CHAN_B_TRIG_LED_PIN 10
#define CHAN_C_TRIG_LED_PIN 27
#define CHAN_D_TRIG_LED_PIN 26

#define TOUCH_INT PC_8

#define ISR_ID_TOGGLE_SWITCHES 0
#define ISR_ID_TACTILE_BUTTONS 1
#define ISR_ID_TOUCH_PADS 2

enum CHAN
{
    A,
    B,
    C,
    D,
    ALL
};
typedef enum CHAN CHAN;