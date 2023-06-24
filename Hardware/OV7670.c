#include "OV7670.h"
#include "gd32f4xx.h"
#include "systick.h"
#include "LCD.h"

#define OV7670_REG_NUM  184
extern uint16_t capture_image[320*240];
// extern uint16_t try_image[320*240];

/*!
    \brief      configure the DCI to interface with the camera module
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dci_config(void)
{
    dci_parameter_struct dci_struct;
    dma_multi_data_parameter_struct dma_single_struct;
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOG);
    rcu_periph_clock_enable(RCU_DCI);
    
    /* DCI GPIO AF configuration */ 
    gpio_af_set(GPIOA, GPIO_AF_13, GPIO_PIN_4);
    gpio_af_set(GPIOA, GPIO_AF_13, GPIO_PIN_6);
    gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_6);
    gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_7);
    gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_8);
    gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_9);
    gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_11);
    gpio_af_set(GPIOB, GPIO_AF_13, GPIO_PIN_8);
    gpio_af_set(GPIOB, GPIO_AF_13, GPIO_PIN_9);
    gpio_af_set(GPIOD, GPIO_AF_13, GPIO_PIN_3);
    gpio_af_set(GPIOG, GPIO_AF_13, GPIO_PIN_9);

    /* configure DCI_PIXCLK(PA6), DCI_VSYNC(PG9), DCI_HSYNC(PA4), DCI_D0(PC6), DCI_D1(PC7)
                 DCI_D2(PC8), DCI_D3(PC9), DCI_D4(PC11), DCI_D5(PD3), DCI_D6(PB8), DCI_D7(PB9) */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_4);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_8);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_11);
    
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_8);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);
    
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3);

    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);
    
    /* DCI configuration */ 
    dci_struct.capture_mode = DCI_CAPTURE_MODE_CONTINUOUS;
    dci_struct.clock_polarity =  DCI_CK_POLARITY_RISING;
    dci_struct.hsync_polarity = DCI_HSYNC_POLARITY_LOW;
    dci_struct.vsync_polarity = DCI_VSYNC_POLARITY_LOW;
    dci_struct.frame_rate = DCI_FRAME_RATE_ALL;
    dci_struct.interface_format = DCI_INTERFACE_FORMAT_8BITS;
    dci_init(&dci_struct);

    /* DCI DMA configuration */ 
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(DMA1,DMA_CH7);
    dma_single_struct.periph_addr= (uint32_t)DCI_DR_ADDRESS;
    // dma_single_struct.periph_addr= (uint32_t)(&try_image);
    dma_single_struct.periph_width = DMA_PERIPH_WIDTH_32BIT;    
    dma_single_struct.periph_inc= DMA_PERIPH_INCREASE_DISABLE;

    dma_single_struct.memory0_addr= (uint32_t)(&capture_image);
    dma_single_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;    
    dma_single_struct.memory_inc= DMA_MEMORY_INCREASE_ENABLE;
    
    dma_single_struct.memory_burst_width = DMA_MEMORY_BURST_SINGLE;
    dma_single_struct.periph_burst_width = DMA_PERIPH_BURST_SINGLE;
    dma_single_struct.critical_value = DMA_FIFO_1_WORD;

    dma_single_struct.circular_mode= DMA_CIRCULAR_MODE_ENABLE;
    dma_single_struct.direction= DMA_PERIPH_TO_MEMORY;
    dma_single_struct.number= 320*240/2;
    dma_single_struct.priority= DMA_PRIORITY_HIGH;

    dma_multi_data_mode_init(DMA1,DMA_CH7,&dma_single_struct);

    dma_channel_subperipheral_select(DMA1,DMA_CH7,DMA_SUBPERI1);
}

const uint8_t OV7670_reg[OV7670_REG_NUM][2]=//OV7670 传感器寄存器、初始化相关设置
{	 
// 		/*以下为OV7670 QVGA RGB565参数  */
// 		{0x3a, 0x04},//dummy
// 		{0x40, 0x10},//565
// 		{0x12, 0x14},//@QVGA=0x14;VGA=0x04?
// 		{0x32, 0x80},//HREF control	bit[2:0] HREF start 3 LSB	 bit[5:3] HSTOP HREF end 3LSB
// 		{0x17, 0x16},//              HSTART start high 8-bit MSB
// 		{0x18, 0x04},//5	 HSTOP end high 8-bit
// 		{0x19, 0x02},
// 		{0x1a, 0x7b},//0x7a,
// 		{0x03, 0x06},//0x0a,
// 		{0x0c, 0x0c},
// 		{0x15, 0x02},
// 		{0x3e, 0x00},//10
// 		{0x70, 0x00},
// 		{0x71, 0x01},
// 		{0x72, 0x11},
// 		{0x73, 0x09},//
// 		{0xa2, 0x02},//15
// 		{0x11, 0x00}, //fenpin--03（普通）--00(较快)--80（花了）---分频(me)
// 		{0x7a, 0x20},
// 		{0x7b, 0x1c},
// 		{0x7c, 0x28},
// 		{0x7d, 0x3c},//20
// 		{0x7e, 0x55},
// 		{0x7f, 0x68},
// 		{0x80, 0x76},
// 		{0x81, 0x80},
// 		{0x82, 0x88},
// 		{0x83, 0x8f},
// 		{0x84, 0x96},
// 		{0x85, 0xa3},
// 		{0x86, 0xaf},
// 		{0x87, 0xc4},//30
// 		{0x88, 0xd7},
// 		{0x89, 0xe8},
// 		{0x13, 0xe0},
// 		{0x00, 0x00},//AGC
// 		{0x10, 0x00},
// 		{0x0d, 0x00},
// 		{0x14, 0x20},//0x38, limit the max gain
// 		{0xa5, 0x05},
// 		{0xab, 0x07},
// 		{0x24, 0x75},//40
// 		{0x25, 0x63},
// 		{0x26, 0xA5},
// 		{0x9f, 0x78},
// 		{0xa0, 0x68},
// 		{0xa1, 0x03},//0x0b,
// 		{0xa6, 0xdf},//0xd8,
// 		{0xa7, 0xdf},//0xd8,
// 		{0xa8, 0xf0},
// 		{0xa9, 0x90},
// 		{0xaa, 0x94},//50
// 		{0x13, 0xe5},
// 		{0x0e, 0x61},
// 		{0x0f, 0x4b},
// 		{0x16, 0x02},
// 		{0x1e, 0x37},//0x07,
// 		{0x21, 0x02},
// 		{0x22, 0x91},
// 		{0x29, 0x07},
// 		{0x33, 0x0b},
// 		{0x35, 0x0b},//60
// 		{0x37, 0x1d},
// 		{0x38, 0x71},
// 		{0x39, 0x2a},
// 		{0x3c, 0x78},
// 		{0x4d, 0x40},
// 		{0x4e, 0x20},
// 		{0x69, 0x5d},
// 		{0x6b, 0x40},//PLL
// 		{0x74, 0x19},
// 		{0x8d, 0x4f},
// 		{0x8e, 0x00},//70
// 		{0x8f, 0x00},
// 		{0x90, 0x00},
// 		{0x91, 0x00},
// 		{0x92, 0x00},//0x19,//0x66
// 		{0x96, 0x00},
// 		{0x9a, 0x80},
// 		{0xb0, 0x84},
// 		{0xb1, 0x0c},
// 		{0xb2, 0x0e},
// 		{0xb3, 0x82},//80
// 		{0xb8, 0x0a},
// 		{0x43, 0x14},
// 		{0x44, 0xf0},
// 		{0x45, 0x34},
// 		{0x46, 0x58},
// 		{0x47, 0x28},
// 		{0x48, 0x3a},
// 		{0x59, 0x88},
// 		{0x5a, 0x88},
// 		{0x5b, 0x44},//90
// 		{0x5c, 0x67},
// 		{0x5d, 0x49},
// 		{0x5e, 0x0e},
// 		{0x64, 0x04},
// 		{0x65, 0x20},
// 		{0x66, 0x05},
// 		{0x94, 0x04},
// 		{0x95, 0x08},
// 		{0x6c, 0x0a},
// 		{0x6d, 0x55},
// 		{0x4f, 0x80},
// 		{0x50, 0x80},
// 		{0x51, 0x00},
// 		{0x52, 0x22},
// 		{0x53, 0x5e},
// 		{0x54, 0x80},
// 		//{0x54, 0x40},//110
// 		{0x6e, 0x11},//100
// 		//{0x13, 0xe7},																	//自己加的--me//AWB、AGC、AGC Enable and ...
// 		{0x6f, 0x9f},//0x9e for advance AWB												9f
// 		{0x55, 0x00},//亮度																0x00
// 		{0x56, 0x50},//对比度													初始值：0x20
// 		{0x57, 0x40},//0x40,  change according to Jim's request	      
{0x3a, 0x04},//dummy
{0x40, 0x10},//565   
{0x12, 0x14},//QVGA,RGB

{0x32, 0x80},//HREF control	bit[2:0] HREF start 3 LSB	bit[5:3] HSTOP HREF end 3LSB
{0x17, 0x16},//HSTART start high 8-bit MSB         
{0x18, 0x04},//5 HSTOP end high 8-bit
{0x19, 0x02},
{0x1a, 0x7b},//0x7a,
{0x03, 0x06},//0x0a,???ú?±·??ò????

{0x0c, 0x0c},
{0x15, 0x02},//0x00
{0x3e, 0x00},//10
{0x70, 0x00},
{0x71, 0x01},
{0x72, 0x11},
{0x73, 0x09},//
        
{0xa2, 0x02},//15
{0x11, 0x00},//
{0x7a, 0x20},
{0x7b, 0x1c},
{0x7c, 0x28},
        
{0x7d, 0x3c},//20
{0x7e, 0x55},
{0x7f, 0x68},
{0x80, 0x76},
{0x81, 0x80},
        
{0x82, 0x88},
{0x83, 0x8f},
{0x84, 0x96},
{0x85, 0xa3},
{0x86, 0xaf},
        
{0x87, 0xc4},//30
{0x88, 0xd7},
{0x89, 0xe8},
{0x13, 0xe0},
{0x00, 0x00},//AGC
        
{0x10, 0x00},
{0x0d, 0x00},//
{0x14, 0x20},//0x38, limit the max gain
{0xa5, 0x05},
{0xab, 0x07},
        
{0x24, 0x75},//40
{0x25, 0x63},
{0x26, 0xA5},
{0x9f, 0x78},
{0xa0, 0x68},
        
{0xa1, 0x03},//0x0b,
{0xa6, 0xdf},//0xd8,
{0xa7, 0xdf},//0xd8,
{0xa8, 0xf0},
{0xa9, 0x90},
        
{0xaa, 0x94},//50
{0x13, 0xe5},
{0x0e, 0x61},
{0x0f, 0x4b},
{0x16, 0x02},
        
{0x1e, 0x37},//
{0x21, 0x02},
{0x22, 0x91},
{0x29, 0x07},
{0x33, 0x0b},
        
{0x35, 0x0b},//60
{0x37, 0x1d},
{0x38, 0x71},
{0x39, 0x2a},
{0x3c, 0x78},
        
{0x4d, 0x40},
{0x4e, 0x20},
{0x69, 0x5d},
{0x6b, 0x40},//PLL*4=48Mhz
{0x74, 0x19},
{0x8d, 0x4f},
        
{0x8e, 0x00},//70
{0x8f, 0x00},
{0x90, 0x00},
{0x91, 0x00},
{0x92, 0x00},//0x19,//0x66
        
{0x96, 0x00},
{0x9a, 0x80},
{0xb0, 0x84},
{0xb1, 0x0c},
{0xb2, 0x0e},
        
{0xb3, 0x82},//80
{0xb8, 0x0a},
{0x43, 0x14},
{0x44, 0xf0},
{0x45, 0x34},
        
{0x46, 0x58},
{0x47, 0x28},
{0x48, 0x3a},
{0x59, 0x88},
{0x5a, 0x88},
        
{0x5b, 0x44},//90
{0x5c, 0x67},
{0x5d, 0x49},
{0x5e, 0x0e},
{0x64, 0x04},
{0x65, 0x20},
        
{0x66, 0x05},
{0x94, 0x04},
{0x95, 0x08},
{0x6c, 0x0a},
{0x6d, 0x55},
        
        
{0x4f, 0x80},
{0x50, 0x80},
{0x51, 0x00},
{0x52, 0x22},
{0x53, 0x5e},
{0x54, 0x80},//偏色设置，偏绿，偏蓝。。。初始值0x80,车牌用0xA0(160)

{0x09, 0x03},

{0x6e, 0x11},//100
{0x6f, 0x9e},//0x9e for advance AWB
{0x55, 0x20},//亮度
{0x56, 0x50},//对比度
{0x57, 0x40},//0x40,  change according to Jim's request 

{0x6a, 0x40},//--下面部分寄存器的设置，有自动白平衡的功能--me
{0x01, 0x40},
{0x02, 0x40},
{0x13, 0xe7},
{0x15, 0x02},  


{0x58, 0x9e},

{0x41, 0x08},
{0x3f, 0x00},
{0x75, 0x05},
{0x76, 0xe1},
{0x4c, 0x00},
{0x77, 0x01},
{0x3d, 0xc2}, 
{0x4b, 0x09},
{0xc9, 0x60},
{0x41, 0x38},

{0x34, 0x11},
{0x3b, 0x02}, 

{0xa4, 0x89},
{0x96, 0x00},
{0x97, 0x30},
{0x98, 0x20},
{0x99, 0x30},
{0x9a, 0x84},
{0x9b, 0x29},
{0x9c, 0x03},
{0x9d, 0x4c},
{0x9e, 0x3f},
{0x78, 0x04},

{0x79, 0x01},
{0xc8, 0xf0},
{0x79, 0x0f},
{0xc8, 0x00},
{0x79, 0x10},
{0xc8, 0x7e},
{0x79, 0x0a},
{0xc8, 0x80},
{0x79, 0x0b},
{0xc8, 0x01},
{0x79, 0x0c},
{0xc8, 0x0f},
{0x79, 0x0d},
{0xc8, 0x20},
{0x79, 0x09},
{0xc8, 0x80},
{0x79, 0x02},
{0xc8, 0xc0},
{0x79, 0x03},
{0xc8, 0x40},
{0x79, 0x05},
{0xc8, 0x30},
{0x79, 0x26}, 
{0x09, 0x00},

{0x13, 0xe7},//banding filer enable
{0x9d, 0x98},//50Hz banding filer
{0x9e, 0x7f},//60Hz banding filer
{0xa5, 0x02},//3 step for 50Hz
{0xab, 0x03},//4 step for 60Hz
{0x3b, 0x02},//select 60Hz banding filer
};

void OV7670_GPIO_Init(void)
{
	//时钟使能
	rcu_periph_clock_enable(RCU_GPIOA);	
	rcu_periph_clock_enable(RCU_GPIOB);	
	rcu_periph_clock_enable(RCU_GPIOC);	
    rcu_periph_clock_enable(RCU_GPIOD);	
    rcu_periph_clock_enable(RCU_GPIOG);	

	//配置摄像头外部时钟-PA8
	gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_8);
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_8);
	/* enable SYSCFG clock */
	rcu_periph_clock_enable(RCU_SYSCFG);  
	/* choose DIV2 to get 4MHz from 16MHz on CKOUT0 pin (PA8) to clock the PHY */
	rcu_ckout0_config(RCU_CKOUT0SRC_HXTAL, RCU_CKOUT0_DIV1);

	//RST-PB3-3.3v
	gpio_mode_set(GPIOB,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_3);
	gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_3);
	gpio_bit_set(GPIOB,GPIO_PIN_3);

	//PWDN-PB4-GND
	gpio_mode_set(GPIOB,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_4);
	gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_4);
	gpio_bit_reset(GPIOB,GPIO_PIN_4);
    
    // /* configure DCI_PIXCLK(PA6), DCI_VSYNC(PG9), DCI_HSYNC(PA4), DCI_D0(PC6), DCI_D1(PC7)
    //              DCI_D2(PC8), DCI_D3(PC9), DCI_D4(PC11), DCI_D5(PD3), DCI_D6(PB8), DCI_D7(PB9) */
    // gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_4);//HS-PA4
    // gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_6);//PCLK-PA6
    // gpio_mode_set(GPIOG, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);//VS-PG9
    
    // gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    // gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    // gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_8);
    // gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    // gpio_mode_set(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_11);
    
    // gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_8);
    // gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    
    // gpio_mode_set(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_3);
}
 
void OV7670_Init(void)
{
	int i;
	OV7670_GPIO_Init();
	dci_config();
    SCCB_Init();
	for(i=0;i<OV7670_REG_NUM;i++)
	{
		OV7670_WriteReg(OV7670_reg[i][0],OV7670_reg[i][1]);
	}		
}


void OV7670_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	SCCB_Start();
	
	SCCB_SendByte(OV7670_ADDRESS);
	SCCB_ReceiveAck();			//SCCB_ReceiveAck()==0
	
	SCCB_SendByte(RegAddress);
	SCCB_ReceiveAck();
	
	SCCB_SendByte(Data);
	SCCB_ReceiveAck();
	
	SCCB_Stop();
}


uint8_t OV7670_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	SCCB_Start();
	SCCB_SendByte(OV7670_ADDRESS);
	SCCB_ReceiveAck();			//SCCB_ReceiveAck()==0
	SCCB_SendByte(RegAddress);
	SCCB_ReceiveAck();
	SCCB_Stop();		//
	
	SCCB_Start();
	SCCB_SendByte(OV7670_ADDRESS | 0x01);
	SCCB_ReceiveAck();
	Data = SCCB_ReceiveByte();
	SCCB_SendNA();
	SCCB_Stop();
	
	return Data;
}

uint8_t OV7670_VS(void)
{
	if(gpio_input_bit_get(GPIOG,GPIO_PIN_9)==SET)
	{
		return 1; 
	}
	if(gpio_input_bit_get(GPIOG,GPIO_PIN_9)==RESET)
	{
		return 0; 
	}	
}

uint8_t OV7670_HREF(void)
{
	if(gpio_input_bit_get(GPIOA,GPIO_PIN_4)==SET)
	{
		return 1; 
	}
	if(gpio_input_bit_get(GPIOA,GPIO_PIN_4)==RESET)
	{
		return 0; 
	}	
}

uint8_t OV7670_PCLK(void)
{
	if(gpio_input_bit_get(GPIOA,GPIO_PIN_6)==SET)
	{
		return 1; 
	}
	if(gpio_input_bit_get(GPIOA,GPIO_PIN_6)==RESET)
	{
		return 0; 
	}	
}

void OV7670_GetPic(void)
{
    uint16_t j,k;
    
    while(OV7670_VS()==0);
    while(OV7670_VS()==1);
    for(j=0;j<240;j++)
    {        
        while(OV7670_HREF()==0);			
        for(k=0;k<320;k++)
        {
            uint16_t color,a;
            /* configure DCI_PIXCLK(PA6), DCI_VSYNC(PG9), DCI_HSYNC(PA4), DCI_D0(PC6), DCI_D1(PC7)
             DCI_D2(PC8), DCI_D3(PC9), DCI_D4(PC11), DCI_D5(PD3), DCI_D6(PB8), DCI_D7(PB9) */
            color = 0x0000;
            while(OV7670_PCLK()==0);
            a = gpio_input_bit_get(GPIOC,GPIO_PIN_6); 
            a = a;
            color = color | a;
            a = gpio_input_bit_get(GPIOC,GPIO_PIN_7); 
            a = a<<1;
            color = color | a;
            a = gpio_input_bit_get(GPIOC,GPIO_PIN_8); 
            a = a<<2;
            color = color | a;
            a = gpio_input_bit_get(GPIOC,GPIO_PIN_9); 
            a = a<<3;
            color = color | a;
            a = gpio_input_bit_get(GPIOC,GPIO_PIN_11); 
            a = a<<4;
            color = color | a;
            a = gpio_input_bit_get(GPIOD,GPIO_PIN_3); 
            a = a<<5;
            color = color | a;
            a = gpio_input_bit_get(GPIOB,GPIO_PIN_8); 
            a = a<<6;
            color = color | a;
            a = gpio_input_bit_get(GPIOB,GPIO_PIN_9); 
            a = a<<7;
            color = color | a;
            while(OV7670_PCLK()==1);
            while(OV7670_PCLK()==0);
            a = gpio_input_bit_get(GPIOC,GPIO_PIN_6); 
            a = a<<8;
            color = color | a;
            a = gpio_input_bit_get(GPIOC,GPIO_PIN_7); 
            a = a<<9;
            color = color | a;
            a = gpio_input_bit_get(GPIOC,GPIO_PIN_8); 
            a = a<<10;
            color = color | a;
            a = gpio_input_bit_get(GPIOC,GPIO_PIN_9); 
            a = a<<11;
            color = color | a;
            a = gpio_input_bit_get(GPIOC,GPIO_PIN_11); 
            a = a<<12;
            color = color | a;
            a = gpio_input_bit_get(GPIOD,GPIO_PIN_3); 
            a = a<<13;
            color = color | a;
            a = gpio_input_bit_get(GPIOB,GPIO_PIN_8); 
            a = a<<14;
            color = color | a;
            a = gpio_input_bit_get(GPIOB,GPIO_PIN_9); 
            a = a<<15;
            color = color | a;
            LCD_Fast_DrawPoint(k,j,color);
            while(OV7670_PCLK()==1);
        }
        while(OV7670_HREF()==1);
    }
    while(OV7670_VS()==0);  
}

void OV7670_config_window(uint16_t startx,uint16_t starty,uint16_t width, uint16_t height)
{
    uint16_t endx=(startx+width*2)%784;
    uint16_t endy=(starty+height*2);
    uint8_t x_reg, y_reg;
    uint8_t state,temp;

    x_reg = OV7670_ReadReg(0x32);
    x_reg &=  0xC0;
    y_reg = OV7670_ReadReg(0x03);
    y_reg &=  0xF0;

    //设置 HREF
    temp = x_reg|((endx&0x7)<<3)|(startx&0x7);
    OV7670_WriteReg(0x32, temp );
    temp = (startx&0x7F8)>>3;
    OV7670_WriteReg(0x17, temp );
    temp = (endx&0x7F8)>>3;
    OV7670_WriteReg(0x18, temp );

    //设置 VREF
    temp = y_reg|((endy&0x3)<<2)|(starty&0x3);
    OV7670_WriteReg(0x03, temp );
    temp = (starty&0x3FC)>>2;
    OV7670_WriteReg(0x19, temp );
    temp = (endy&0x3FC)>>2;
    OV7670_WriteReg(0x1A, temp );
}
