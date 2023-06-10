#ifndef _USART_H
#define _USART_H

#include "gd32f4xx.h"
#include "systick.h"

void usart_init(int baud_rate);
void usart_send_data(uint8_t ucch);
void usart_send_string(uint8_t *data);


#endif
