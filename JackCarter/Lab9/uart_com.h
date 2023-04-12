/*
 * uart_com.h
 *
 *  Created on: Feb 22, 2023
 *      Author: jackmorr
 */

#ifndef UART_COM_H_
#define UART_COM_H_

void uart_init();
void uart_send(char data);
char uart_recieve();
void uart_sendString(char *s);



#endif /* UART_COM_H_ */
