/**
 *
 * @file
 *
 * @brief UDP socket interface
 *
 * Copyright (c) 2014 Proximetry Inc. All rights reserved.
 * Copyright (c) 2014 Atmel Corporation. All rights reserved.
 *
 * @asf_license_start
 *
 * @page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @asf_license_stop
 *
 */

#ifndef SOCKET_LAYER_H
#define SOCKET_LAYER_H

#include <stdbool.h>
#include <stdint.h>
#include "socket/include/socket.h"
#include "prox_agent_common.h"

#define SOCKET_DEBUG							1

typedef void (*parse_msg_api) (struct sockaddr_in *src, uint8_t *buff, size_t len);

typedef struct {
	SOCKET sock;
	parse_msg_api cb;
	struct sockaddr_in remote;
	struct sockaddr_in local;
} socket_desc;

/**
 * @brief Initializes mcu sockets layer
 *
 * @return true on success, false on failure
 */
bool winc1500_socket_layer_init(void);

/**
 * @brief De-initialize sockets
 */
void winc1500_socket_layer_deinit(void);

/**
 * @brief Checks if sockets are ready to be used
 *
 * @return true if layer is active, false otherwise
 */
bool winc1500_is_socket_layer_active(void) __attribute__((pure));

/**
 * @brief Registers a function the will be called upon receipt
 *
 * @paramp[in] sock socket descriptor
 * @param[in] cb    callback meant to parse the given messages
 */
void winc1500_socket_layer_register_recv_cb(socket_desc *sock, parse_msg_api cb);

/**
 * @brief Initializes socket
 *
 * @return socket if initialization succeeded, NULL otherwise
 */
socket_desc *winc1500_socket(uint16 u16Domain, uint8 u8Type, uint8 u8Flags);

/**
 * @brief Close socket
 *
 * @param[in] sock Socket to close
 *
 * @return Status of operation
 */
int winc1500_socket_close(socket_desc *sock);

/**
 * @brief Connect socket to address
 *
 * @param[in] sock Socket to update
 * @param[in] addr Address to connect to
 *
 * @return Updated socket descriptor
 */
socket_desc *winc1500_socket_connect(socket_desc *sock,struct sockaddr_in *addr);

/**
 * @brief Bind socket to address
 *
 * @param[in] sock Socket to bind
 * @param[in] addr Address to bind to
 *
 * @return Updated socket
 */
socket_desc *winc1500_socket_bind(socket_desc *sock,struct sockaddr_in *addr);

/**
 * @brief Sends a packet via the UDP socket
 *
 * @param[in] sock      socket to send packet via
 * @param[in] msg       buffer with the message body
 * @param[in] msg_len   message body length
 *
 * @return 0 on success, a negative number otherwise
 */
int16_t winc1500_udp_send(socket_desc *sock, uint8_t *msg, uint16_t msg_len);

/**
 * @brief Sends a packet via the UDP socket
 *
 * @param[in] sock      socket to send packet via
 * @param[in] addr      address to send message to
 * @param[in] msg       buffer with the message body
 * @param[in] msg_len   message body length
 *
 * @return 0 on success, a negative number otherwise
 */
int16_t winc1500_udp_sendto(socket_desc *sock, struct sockaddr_in *addr, uint8_t *msg, uint16_t msg_len);

#endif /* SOCKET_LAYER_H */

