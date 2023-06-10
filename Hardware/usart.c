#include "usart.h"
#include <stdio.h>

void usart_init(int baud_rate)
{
//配置GPIO
//开启时钟
	rcu_periph_clock_enable(RCU_USART0); // 开启串口时钟
	rcu_periph_clock_enable(RCU_GPIOA); // 开启端口时钟 
//配置GPIO复用模式
	gpio_af_set(GPIOA,GPIO_AF_7,GPIO_PIN_9);
	gpio_af_set(GPIOA,GPIO_AF_7,GPIO_PIN_10);
//配置GPIO输出模式
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,  GPIO_PIN_9);
	gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,  GPIO_PIN_10);
//配置GPIO的输出
	gpio_output_options_set( GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_output_options_set( GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
//配置串口
	usart_deinit(USART0);//复位串口
	usart_baudrate_set(USART0,baud_rate);//配置波特率
	usart_parity_config(USART0, USART_PM_NONE);//配置校验位
	usart_stop_bit_set(USART0,USART_STB_1BIT);//配置串口的停止位
	usart_word_length_set(USART0,USART_WL_8BIT);//配置串口的数据长度
//使能串口
	usart_enable(USART0);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);//打开串口的发送功能
}

void usart_send_data(uint8_t ucch)
{
	usart_data_transmit(USART0,(uint8_t)ucch);							 // 发送数据
	while(RESET == usart_flag_get(USART0,USART_FLAG_TBE));  // 等待发送数据缓冲区标志置位
}

//串口发送字符串 

void usart_send_string(uint8_t *data)
{
	while(data && *data)
	{
		usart_send_data(*data++);
	}
}

int fputc(int ch, FILE *f)
{ 
	usart_data_transmit(USART0, (uint8_t)ch); 
	while(RESET == usart_flag_get(USART0, USART_FLAG_TBE)); // 等待发送数据缓冲 区标志置位
	return ch; 
}
