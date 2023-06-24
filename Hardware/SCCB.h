#ifndef __SCCB_H
#define __SCCB_H

#include "gd32f4xx.h"                  // Device header

 /* configure I2C0 pins: PB6 and PB7 */ 
#define DCI_SCCB                       I2C0
#define DCI_SCCB_CLK                   RCU_I2C0

#define DCI_SCCB_SDA_PIN               GPIO_PIN_7
#define DCI_SCCB_SDA_GPIO_PORT         GPIOB
#define DCI_SCCB_SDA_GPIO_CLK          RCU_GPIOB

#define DCI_SCCB_SCL_PIN               GPIO_PIN_6
#define DCI_SCCB_SCL_GPIO_PORT         GPIOB
#define DCI_SCCB_SCL_GPIO_CLK          RCU_GPIOB

#define SCCB_FLAG_TIMEOUT              20000000
#define OV7670_DEVICE_WRITE_ADDRESS    0x42
#define OV7670_DEVICE_READ_ADDRESS     0x43

#define SCCB_SPEED                     100000
#define SCCB_SLAVE_ADDRESS7            0xFE 

/* function declarations */
void sccb_config(void);
uint8_t dci_byte_write(uint8_t reg, uint8_t data);
uint8_t dci_byte_read(uint8_t reg, uint8_t *data);

void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_SendByte(uint8_t Byte);
uint8_t SCCB_ReceiveByte(void);
void SCCB_SendNA(void);
uint8_t SCCB_ReceiveAck(void);

#endif
