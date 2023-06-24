/*!
    \file    main.c
    \brief   led spark with systick

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
    \version 2022-03-09, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f4xx.h"
#include "systick.h"
#include "usart.h"
#include "lcd.h"
#include "touch.h"
#include "OV7670.h"
#include "SCCB.h"
#include "picture.h"
#include "tinymaix.h"
#include <stdio.h>
#include "main.h"

uint16_t POINT[5]={WHITE,BLUE,RED,YELLOW,GREEN};
uint16_t BACK[5]={LIGHTGREEN,LIGHTGRAY,LGRAY,LGRAYBLUE,LBBLUE};
uint16_t capture_image[320*240];
// uint16_t try_image[320*240];

//int classification_main(int argc, char** argv);
int main_detection(int argc, char** argv);
void LCD_WriteRAM_Prepare(void);

/*!
    \brief    main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint8_t temp,error;    
    uint8_t t = 0;
    uint16_t lastpos[5][2]; 
    uint16_t i,color_l;
    uint16_t color_h;
    uint32_t color;
    // for(int i;i<320*240;i++){
    //     color++;
    //     try_image[i] = color;
    // }

    // 定时器初始化
    systick_config();
    //串口初始化
    usart_init(115200);      
    //LCD初始化
    LCD_Init();
    POINT_COLOR=POINT[4];
    BACK_COLOR=BACK[4];
    LCD_Clear(0x6B4D);
    // LCD_ShowString(30,50,480,80,24,1,"https://lckfb.com");     
    // LCD_ShowString(30,80,480,110,24,1,lcd_id);
    // LCD_ShowString(30,110,480,140,24,1,"touch test....");
    POINT_COLOR = 0x0000;
    LCD_ShowString(180,620,160,80,24,1,"Detection");
    LCD_DrawFrame(160,600,160,80,0x0000);
    // LCD_ShowString(160,600,160,80,32,1,"Detection");
    //触摸屏
    GT1151_Init(); 
    //摄像头初始化    
    OV7670_Init();   
    delay_1ms(100);  

    //OV7670_WriteReg(0x11, 0x9F);/* 输入时钟32分频 */
    OV7670_WriteReg(0x6b, 0x3A);/* 设置输入时钟倍频为1 */
    // OV7670_WriteReg(0x70, 0xBA);
    // OV7670_WriteReg(0x71, 0xB5);
    OV7670_config_window(184,10,320,240);

    dci_enable();
    dma_channel_enable(DMA1, DMA_CH7);
    dci_capture_enable();

    delay_1ms(100);

    //神经网络测试

    while(1) {            
        for (int j=0;j<240;++j)
        {	
            for (int k=0;k<320;++k)
            {     
                LCD_Fast_DrawPoint(k+80,j+100,capture_image[320*j+k]);  
            }
        }
        GT1151_Scan(0);
        if (tp_dev.x[0]>160 && tp_dev.x[0]<160+160 && tp_dev.y[0]>600 && tp_dev.y[0]<600+80){
            main_detection(0,0);
            POINT_COLOR = POINT[2];
            LCD_DrawFrame(120,100,200,220,POINT_COLOR);
            POINT_COLOR = 0x0000;
            LCD_ShowString(200,400,480,140,24,1,"Truman Xia");
        }
        }
}
