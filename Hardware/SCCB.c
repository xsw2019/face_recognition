#include "gd32f4xx.h"                  // Device header
#include "systick.h"
#include "SCCB.h"

/*
  * @brief  修改SCL的电平
  * @param  0或者1
  * @retval 无
*/
void SCCB_W_SCL(uint8_t BitValue)
{
	if(BitValue == 0)
	{
		gpio_bit_reset(GPIOB,GPIO_PIN_5);
	}
	if(BitValue == 1)
	{
		gpio_bit_set(GPIOB,GPIO_PIN_5);
	}
	delay_1us(10);
}
/*
  * @brief  修改SDA的电平
  * @param  0或者1
  * @retval 无
*/
void SCCB_W_SDA(uint8_t BitValue)
{
	if(BitValue == 0x00)
	{
		gpio_bit_reset(GPIOB,GPIO_PIN_13);
	}
	if(BitValue != 0x00)
	{
		gpio_bit_set(GPIOB,GPIO_PIN_13);
	}
	delay_1us(10);
}
/*
  * @brief  设置SDA为输入模式
  * @param  无
  * @retval 无
*/
void SCCB_SDA_IN(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
	gpio_mode_set(GPIOB,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_PIN_13);//SDA PB13
    
	gpio_bit_set(GPIOB,GPIO_PIN_13);
}
/*
  * @brief  设置SDA为输出模式
  * @param  无
  * @retval 无
*/
void SCCB_SDA_OUT(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);	
	gpio_mode_set(GPIOB,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_13);
	gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_13);//SDA PB13
    
	gpio_bit_set(GPIOB,GPIO_PIN_13);
}

/*
  * @brief  读取SDA的电平
  * @param  无
  * @retval 0或者1
*/
uint8_t SCCB_R_SDA(void)
{
	uint8_t BitValue;
	if(gpio_input_bit_get(GPIOB,GPIO_PIN_13)==SET)
	{
		BitValue = 1; 
	}
	if(gpio_input_bit_get(GPIOB,GPIO_PIN_13)==RESET)
	{
		BitValue = 0; 
	}	
	delay_1us(10);
	return BitValue;
}
/*
  * @brief  SCCB初始化
  * @param  无
  * @retval 无
*/
void SCCB_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
		
	gpio_mode_set(GPIOB,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_5);
	gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_5);//SCL PB5

	gpio_mode_set(GPIOB,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,GPIO_PIN_13);
	gpio_output_options_set(GPIOB,GPIO_OTYPE_PP,GPIO_OSPEED_MAX,GPIO_PIN_13);//SDA PB13
}
/*
  * @brief  产生SCCB开始信号
  * @param  无
  * @retval 无
*/
void SCCB_Start(void)
{
	SCCB_W_SDA(1);
	delay_1us(10);
	SCCB_W_SCL(1);
	delay_1us(2);
	SCCB_W_SDA(0);
	delay_1us(2);
	SCCB_W_SCL(0);
}
/*
  * @brief  产生SCCB结束信号
  * @param  无
  * @retval 无
*/
void SCCB_Stop(void)
{
	SCCB_W_SDA(0);
	SCCB_W_SCL(1);
	SCCB_W_SDA(1);
}
/*
  * @brief	SCCB发送一个字节
  * @param  一个字节数据
  * @retval 无
*/
void SCCB_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{	
		delay_1us(2);
		SCCB_W_SDA(Byte & (0x80 >> i));
		delay_1us(2);
		SCCB_W_SCL(1);
		delay_1us(2);
		SCCB_W_SCL(0);
		delay_1us(2);
	}
}
/*
  * @brief	SCCB接收一个字节
  * @param  无
  * @retval 接收到的字节
*/
uint8_t SCCB_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	SCCB_SDA_IN();/* 转换SDA为输入模式 */

	for (i = 0; i < 8; i ++)
	{
		SCCB_W_SCL(1);
		if (SCCB_R_SDA() == 1){Byte |= (0x80 >> i);}
		SCCB_W_SCL(0);
		delay_1us(1);
	}
	SCCB_SDA_OUT();
	return Byte;
}
/*
  * @brief	SCCB发送NA信号
  * @param  无
  * @retval 无
*/
void SCCB_SendNA()
{
	SCCB_W_SDA(1);
	SCCB_W_SCL(1);
	SCCB_W_SCL(0);
	SCCB_W_SDA(0);//new
}
/*
  * @brief	SCCB接收Ack应答
  * @param  无
  * @retval 接收到的应答，若数据成功发送，应答为0，反之为1
*/
uint8_t SCCB_ReceiveAck(void)
{
	uint8_t AckBit;
	SCCB_SDA_IN();/* 转换SDA为输入模式 */
	SCCB_W_SCL(1);
	AckBit = SCCB_R_SDA();
	SCCB_W_SCL(0);
	SCCB_SDA_OUT();/* 转换SDA为输出模式 */
	return AckBit;
}


/*!
    \brief      configure I2C gpio and I2C parameter
    \param[in]  none
    \param[out] none
    \retval     none
*/
void sccb_config(void)
{
    /* enable GPIOB and I2C clock */
    rcu_periph_clock_enable(DCI_SCCB_SCL_GPIO_CLK);
    rcu_periph_clock_enable(DCI_SCCB_CLK);

    /* configure I2C GPIO pins AF */
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_6);
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_7);
    
    /* configure I2C0_SCL(PB6), I2C0_SDA(PB7) */ 
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,DCI_SCCB_SCL_PIN);
    
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,DCI_SCCB_SDA_PIN);
    
    /* configure I2C parameter */
    i2c_deinit(DCI_SCCB);
    i2c_clock_config(DCI_SCCB,SCCB_SPEED,I2C_DTCY_2);
    i2c_mode_addr_config(DCI_SCCB,I2C_I2CMODE_ENABLE,I2C_ADDFORMAT_7BITS,SCCB_SLAVE_ADDRESS7);
    i2c_enable(DCI_SCCB);

    i2c_ack_config(DCI_SCCB,I2C_ACK_ENABLE);
}

/*!
    \brief      write a byte at a specific camera register
    \param[in]  reg:  camera register address
    \param[in]  data: data to be written to the specific register
    \param[out] none
    \retval     0x00 if write operation is OK. 
                0xFF if timeout condition occured (device not connected or bus error).
*/
uint8_t dci_byte_write(uint8_t reg, uint8_t data)
{
    volatile uint32_t timeout = SCCB_FLAG_TIMEOUT;

    /* the software must wait until i2c bus is idle */
    while(i2c_flag_get(DCI_SCCB,I2C_FLAG_I2CBSY));  
    /* generate the start condition */
    i2c_start_on_bus(DCI_SCCB);
    
    /* test on i2c I2C_SBSEND and clear it */
    timeout = SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0)
            return 0xFF;
    }
    
    /* send DCI selected device slave address for write */
    i2c_master_addressing(DCI_SCCB, OV7670_DEVICE_WRITE_ADDRESS,I2C_TRANSMITTER);
    
    /* test on I2C I2C_ADDSEND and clear it */
    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(DCI_SCCB,I2C_FLAG_ADDSEND);
    
    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB,I2C_FLAG_TBE)){
         if ((timeout--) == 0) 
             return 0xFF;
    }
    
    /* send register address */
    i2c_data_transmit(DCI_SCCB, (uint8_t)(reg));

    /* test on I2C I2C_BTC and clear it */
    timeout = SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_BTC)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    /* send data */
    i2c_data_transmit(DCI_SCCB, data);    

    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_TBE)){
        if ((timeout--) == 0)
            return 0xFF;
    }  
    /* send I2C stop condition */
    i2c_stop_on_bus(DCI_SCCB);

    /* if operation is ok, return 0 */
    return 0;
}

/*!
    \brief      read a byte from a specific camera register 
    \param[in]  reg:  camera register address
    \param[out] data: read from the specific register
    \retval     0x00 if write operation is ok. 
                0xFF if timeout condition occured (device not connected or bus error).
*/
uint8_t dci_byte_read(uint8_t reg, uint8_t *data)
{
    volatile uint32_t timeout = SCCB_FLAG_TIMEOUT;

    I2C_STAT0(DCI_SCCB) |= (uint16_t)0x0400;

    /* generate the start condition */
    i2c_start_on_bus(DCI_SCCB);

    /* test on I2C I2C_SBSEND and clear it */
    timeout = SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    /* send DCI selcted device slave address for read */
    i2c_master_addressing(DCI_SCCB, OV7670_DEVICE_READ_ADDRESS,I2C_TRANSMITTER);

    /* test on I2C I2C_ADDSEND and clear it */
    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(DCI_SCCB,I2C_FLAG_ADDSEND);
    
    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB,I2C_FLAG_TBE)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    
    /* send register address */
    i2c_data_transmit(DCI_SCCB, (uint8_t)(reg));
          
    /* test on I2C I2C_BTC and clear it */
    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_BTC)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 

    /* clear AF flag if arised */
    I2C_STAT0(DCI_SCCB) |= (uint16_t)0x0400;
        
    /* prepare stop after receiving data */
    i2c_stop_on_bus(DCI_SCCB);
    
    /* generate the start condition */
    i2c_start_on_bus(DCI_SCCB);

    /* test on I2C I2C_SBSEND and clear it */
    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    
    /* send DCI selcted device slave address for write */
    i2c_master_addressing(DCI_SCCB, OV7670_DEVICE_WRITE_ADDRESS,I2C_RECEIVER);
    
    /* test on I2C I2C_ADDSEND and clear it */
    timeout = SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(DCI_SCCB,I2C_FLAG_ADDSEND); 
    
    /* prepare an nack for the next data received */
    i2c_ack_config(DCI_SCCB, I2C_ACK_DISABLE);

    /* test on I2C_RBNE and clear it */
    timeout = SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_RBNE)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    
    /* prepare stop after receiving data */
    i2c_stop_on_bus(DCI_SCCB); 

    /* receive the data */
    *data = i2c_data_receive(DCI_SCCB);
    /* clear AF flag if arised */
    I2C_STAT0(DCI_SCCB) |= (uint16_t)0x0400;

    return 0;
}
