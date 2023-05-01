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
/*
 * blocks
 */
CommandRequest read_command(void);

/*
 * Sends encoded buffer from Cybot to UI over UART. buf[] needs contain the proper
 * serialized response before it can be sent. Serialize with cyproto.h
 */
void send_response(uint8_t buf[]);

#endif /* CYPROTO_COMPAT_H_ */
