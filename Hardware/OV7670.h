#ifndef _OV7670_H
#define _OV7670_H

#include "SCCB.h"
#include "LCD.h"

#define SCCB_ID   		0X42  			//OV7670 ID
#define OV7670_ADDRESS  0x42

#define DCI_DR_ADDRESS     (0x50050028U)

void OV7670_GPIO_Init(void);
void OV7670_Init(void);
void OV7670_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t OV7670_ReadReg(uint8_t RegAddress);
void OV7670_GetPic(void);
void OV7670_config_window(uint16_t startx,uint16_t starty,uint16_t width, uint16_t height);

#endif 
