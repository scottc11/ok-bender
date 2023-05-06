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

#define I2C3_SDA PC_9
#define I2C3_SCL PA_8
#define I2C1_SDA PB_9
#define I2C1_SCL PB_8

#define SX1509_CHAN_A_ADDR 0x3E
#define SX1509_CHAN_B_ADDR 0x70
#define SX1509_CHAN_C_ADDR 0x3F
#define SX1509_CHAN_D_ADDR 0x71

#define DAC_CS PB_12

#define SPI2_MOSI PB_15
#define SPI2_MISO PB_14
#define SPI2_SCK PB_13

#define ADC_DMA_BUFFER_SIZE 4

#define ADC_A PA_6
#define ADC_B PA_7
#define ADC_C PC_5
#define ADC_D PC_4

#define GATE_OUT_A PA_0
#define GATE_OUT_B PA_1
#define GATE_OUT_C PA_2
#define GATE_OUT_D PA_3

#define REC_LED PA_5
#define CLEAR_LED PA_4
#define RESET_LED PB_0

#define DEGREES_INT PC_6
#define BUTTONS_INT PB_5

#define TOUCH_INT_A PC_0
#define TOUCH_INT_B PC_1
#define TOUCH_INT_C PC_2
#define TOUCH_INT_D PC_3

#define TOUCH_INT_E PC_7

#define MCP23017_DEGREES_ADDR 0x20 // 0100000
#define MCP23017_CTRL_ADDR 0x24    // 0100100

#define ISR_ID_TOGGLE_SWITCHES 0
#define ISR_ID_TACTILE_BUTTONS 1
#define ISR_ID_TOUCH_PADS 2