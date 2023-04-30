/*
 * cyproto_compat.c
 *
 *  Created on: Apr 30, 2023
 *      Author: jackmorr
 */


#include "cyproto.h"
#include "uart_int.h"

void read_command_into(uint8_t buf[]) {
    size_t index = 0;

    while (!uart_hasValue()) {}
    timer_waitMillis(25);
    uint8_t byte = (uint8_t) uart_receiveNonBlocking();
    while (byte != 0) {
        buf[index] = byte;
        index += 1;
        byte = (uint8_t) uart_receiveNonBlocking();
    }
    buf[index] = byte;
}

CommandRequest read_command(void) {
    uint8_t buf[cyproto_buffer_size()];
    read_command(buf);
    return cyproto_parse_command(buf);
}

void send_response(uint8_t buf[]) {
    int i = 0;
    while (buf[i] != 0) {
        uart_sendChar(buf[i++]);
    }
    uart_sendChar(0);
}
