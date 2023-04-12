/*
 * banner.c
 *
 *  Created on: Feb 1, 2023
 *      Author: jackmorr
 */

#include <stdint.h>
#define LCD_WIDTH 20


void __print(char *text, unsigned int offset) {
    unsigned int length = strlen(text);
    if (length <= LCD_WIDTH) {
        lcd_clear();
        lcd_puts(text);
    } else if (offset < length) {
        unsigned int idx = 0;
        lcd_clear();
        while (idx < LCD_WIDTH && idx + offset < length) {
            lcd_putc(text[offset + idx++]);
        }
        int i = 0;
        while (i < LCD_WIDTH - idx) {
            lcd_putc(text[i++]);
        }
    }
}

void bnr_rotating(char *text) {
    uint8_t offset = 0;
    while (1) {
        bannerPrint(text, offset);
        timer_waitMillis(300);
        offset += 1;
        if (offset >= strlen(text)) {
            offset = 0;
        }
    }
}
