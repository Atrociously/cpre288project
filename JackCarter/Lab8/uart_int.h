/*
 * uart_int.h
 *
 *  Created on: Mar 1, 2023
 *      Author: jackmorr
 */

#ifndef UART_INT_H_
#define UART_INT_H_



void uart_interrupt_init(void);
void uart_sendChar(char data);
char uart_receiveNonBlocking(void);
void uart_sendStr(const char *data);


#endif /* UART_INT_H_ */
