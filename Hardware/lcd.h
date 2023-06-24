#ifndef __LCD_H
#define __LCD_H
#include "gd32f4xx.h"
#include "systick.h"


#define LCD_RST_ON  gpio_bit_set(GPIOD,GPIO_PIN_12)     // PD12
#define LCD_RST_OFF gpio_bit_reset(GPIOD,GPIO_PIN_12)   // PD12
#define LCD_BLK_ON  gpio_bit_set(GPIOD,GPIO_PIN_13)     // PD13
#define LCD_BLK_OFF gpio_bit_reset(GPIOD,GPIO_PIN_13)   // PD13


//������ɫ
#define WHITE        0xFFFF
#define BLACK        0x0000	  
#define BLUE         0x001F  
#define BRED         0XF81F
#define GRED         0XFFE0
#define GBLUE        0X07FF
#define RED          0xF800
#define MAGENTA      0xF81F
#define GREEN        0x07E0
#define CYAN         0x7FFF
#define YELLOW       0xFFE0
#define BROWN        0XBC40 //��ɫ
#define BRRED        0XFC07 //�غ�ɫ
#define GRAY         0X8430 //��ɫ
//GUI��ɫ
#define DARKBLUE         0X01CF	//����ɫ
#define LIGHTBLUE        0X7D7C	//ǳ��ɫ  
#define GRAYBLUE         0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
#define LIGHTGREEN       0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY            0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���
#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	 
#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��


//LCD��ַ�ṹ�� 16BIT
typedef struct
{
	uint16_t  LCD_REG;
	uint16_t  LCD_RAM;
} LCD_TypeDef;
#define LCD_BASE    ((uint32_t )(0x6c000000 |  0X000007FE))  //    111 1111 1111
#define LCD         ((LCD_TypeDef *) LCD_BASE)


//LCD��Ҫ������
typedef struct  
{										    
	uint16_t  width;		//LCD ���
	uint16_t  height;		//LCD �߶�
	uint16_t  id;				//LCD ID
	uint8_t   dir;			//���������������ƣ�0��������1��������	
	uint16_t 	wramcmd;	//��ʼдgramָ��
	uint16_t   setxcmd;	//����x����ָ��
	uint16_t   setycmd;	//����y����ָ��  
}_lcd_dev; 	  
//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern uint16_t  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern uint16_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
extern uint8_t  lcd_id[12]; //���LCD ID�ַ���


///////////////
void LCD_CtrlLinesConfig(void);//��ʼ��lcd IO
void LCD_FSMCConfig(void);//FSMC��ʼ��
void LCD_WriteReg(uint16_t  LCD_Reg,uint16_t  LCD_RegValue);//д�Ĵ�������
uint16_t  LCD_ReadReg(uint16_t  LCD_Reg);//��IC�Ĵ�������
void LcdNT35510ReadID(void);//��ID
void LCD_HVGA_NT35510(void);//���ýṹ��
void NT35510_HY35_Initial_Code(void);//������Ļ����
////////////////
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);//Lcd��ѡ����
void LCD_Clear(uint16_t  color);//�������� 
void LCD_Scan_Dir(uint8_t dir);//����LCD���Զ�ɨ�跽��
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);//���ù��λ��
////////////////
void LCD_Init(void);//��Ļ��ʼ��
////////////////
void LCD_DrawPoint(uint16_t x,uint16_t y);                    //����
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);     //���ٻ���
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y);                    //���� 
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);////��ָ��λ����ʾһ���ַ�
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t mode,uint8_t *p);//��ʾ�ַ���
void LCD_DrawFrame(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t color); //����

#endif  




