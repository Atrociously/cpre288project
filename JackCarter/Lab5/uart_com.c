/*
 * uart_com.c
 *
 *  Created on: Feb 22, 2023
 *      Author: jackmorr
 */
#include <string.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>

#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

static int initialized = 0;

void uart_init() {
    if (initialized) {
        return;
    }
    initialized = 1;

    SYSCTL_RCGCGPIO_R |= BIT1;
    SYSCTL_RCGCUART_R |= BIT1;

    GPIO_PORTB_AFSEL_R |= BIT0 | BIT1;
    GPIO_PORTB_PCTL_R |= 0x00000011;
    GPIO_PORTB_DEN_R |= BIT0 | BIT1;
    GPIO_PORTB_DIR_R |= BIT1;
    GPIO_PORTB_DIR_R &= ~BIT0;

    UART1_CTL_R &= ~BIT0; // disable for setup
    // clock rate    clock div     target baud rate   integer
    // (16,000,000)/ (16)       / (115,200)         = 8
    UART1_IBRD_R = 8;
    // (16,000,000)/(16)/(115,200) remainder = 0.6805 * 64 = 44 (rounded)
    UART1_FBRD_R = 44;
    UART1_LCRH_R = UART_LCRH_WLEN_8;
    UART1_CC_R = 0; // set to sysclk

    UART1_CTL_R |= (0x3 << 8) | BIT0; // re-enable uart
}

void uart_send(char data) {
    while (UART1_FR_R & UART_FR_TXFF) {}
    UART1_DR_R = (char)(data & 0xFF);
}

char uart_recieve() {
    while (UART1_FR_R & UART_FR_RXFE) {}
    char data;
    data = (char) (UART1_DR_R & 0xFF);
    return data;
}

void uart_sendString(char *s) {
    int i = 0;
    for (i = 0; i < strlen(s); i++) {
        uart_send(s[i]);
    }
}
