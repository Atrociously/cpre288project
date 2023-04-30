/*
 * cyproto_compat.h
 *
 *  Created on: Apr 30, 2023
 *      Author: jackmorr
 */

#ifndef CYPROTO_COMPAT_H_
#define CYPROTO_COMPAT_H_

#include "cyproto.h"
#include "uart_int.h"

CommandRequest read_command(void);
void send_response(uint8_t buf[]);

#endif /* CYPROTO_COMPAT_H_ */
