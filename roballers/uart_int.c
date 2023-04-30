/*
 * uart_int.c
 *
 *  Created on: Mar 1, 2023
 *      Author: jackmorr
 */
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"

void UART1_Handler(void);

#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80


static int index = 0;
static uint8_t buffer[512];

void uart_interrupt_init(void) {
    //enable clock to GPIO port B
      SYSCTL_RCGCGPIO_R |= BIT1;

      //enable clock to UART1
      SYSCTL_RCGCUART_R |= BIT1;

      //wait for GPIOB and UART1 peripherals to be ready
      while ((SYSCTL_PRGPIO_R & BIT1) == 0) {};
      while ((SYSCTL_PRUART_R & BIT1) == 0) {};

      //enable digital functionality on port B pins
      GPIO_PORTB_DEN_R |= BIT0 | BIT1;
      GPIO_PORTB_DIR_R |= BIT1;
      GPIO_PORTB_DIR_R &= ~BIT0;

      //enable alternate functions on port B pins
      GPIO_PORTB_AFSEL_R |= BIT0 | BIT1;

      //enable UART1 Rx and Tx on port B pins
      GPIO_PORTB_PCTL_R = 0x00000011;

      //calculate baud rate
      uint16_t iBRD = 8; //use equations
      uint16_t fBRD = 44; //use equations

      //turn off UART1 while setting it up
      UART1_CTL_R &= ~BIT0;

      //set baud rate
      //note: to take effect, there must be a write to LCRH after these assignments
      UART1_IBRD_R = iBRD;
      UART1_FBRD_R = fBRD;

      //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
      //note: this write to LCRH must be after the BRD assignments
      UART1_LCRH_R = UART_LCRH_WLEN_8;

      //use system clock as source
      //note from the datasheet UARTCCC register description:
      //field is 0 (system clock) by default on reset
      //Good to be explicit in your code
      UART1_CC_R = 0x0;

      //////Enable interrupts

      //first clear RX interrupt flag (clear by writing 1 to ICR)
      UART1_ICR_R |= 0b00010000;

      //enable RX raw interrupts in interrupt mask register
      UART1_IM_R |= BIT4;

      //NVIC setup: set priority of UART1 interrupt to 1 in bits 21-23
      NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF0FFFFF) | 0x00200000;

      //NVIC setup: enable interrupt for UART1, IRQ #6, set bit 6
      NVIC_EN0_R |= BIT6;

      //tell CPU to use ISR handler for UART1 (see interrupt.h file)
      //from system header file: #define INT_UART1 22
      IntRegister(INT_UART1, UART1_Handler);

      //globally allow CPU to service interrupts (see interrupt.h file)
      IntMasterEnable();

      //re-enable UART1 and also enable RX, TX (three bits)
      //note from the datasheet UARTCTL register description:
      //RX and TX are enabled by default on reset
      //Good to be explicit in your code
      //Be careful to not clear RX and TX enable bits
      //(either preserve if already set or set them)
      UART1_CTL_R = (0x3 << 8) | BIT0;
}

void uart_sendChar(char data) {
    while (UART1_FR_R & UART_FR_TXFF) {}
    UART1_DR_R = (char)(data & 0xFF);
}

bool uart_hasValue(void) {
    return index > 0;
}

uint8_t uart_receiveNonBlocking(void) {
    if (!uart_hasValue()) {
        return 1;
    }
    uint8_t val = buffer[0];
    // shift buffer over
    int i;
    for (i = 0; i < index - 1; i++) {
        buffer[i] = buffer[i + 1];
    }
    index -= 1;
    return val;
}

void uart_sendStr(const char *data) {
    int i = 0;
    for (i = 0; i < strlen(data); i++) {
        uart_sendChar(data[i]);
    }
}

void UART1_Handler(void) {
    char byte_received;
        //check if handler called due to RX event
        if (UART1_MIS_R & BIT4)
        {
            //byte was received in the UART data register
            //clear the RX trigger flag (clear by writing 1 to ICR)
            UART1_ICR_R |= BIT4;

            //read the byte received from UART1_DR_R and echo it back to PuTTY
            //ignore the error bits in UART1_DR_R
            byte_received = (uint8_t) (UART1_DR_R & 0xFF);
            buffer[index] = byte_received;
            index += 1;
        }
}
