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

#include <string.h>
#include "socket_layer.h"
#include "hpl_reset.h"

#define SOCKET_DBG	"SOCKET: "

#ifdef SOCKET_DEBUG
#define SOCKET_DEBUG_LOG LOG
#else
#define SOCKET_DEBUG_LOG LOG_DEBUG
#endif

#define BASE_LOCAL_PORT 25000

static volatile bool socket_layer_active = false;

static bool cb_registered = false;


#define NO_OF_SOCKETS 2
socket_desc sockets[NO_OF_SOCKETS];

// MTU
#define MTU			516
static uint8_t socket_recv_buff[MTU] = {0};


/**
 * @brief Registers a function the will be called upon receipt
 */
inline void winc1500_socket_layer_register_recv_cb(socket_desc *sock, parse_msg_api cb)
{
	sock->cb = cb;
}

/**
 * @brief Logs error messages
 *
 * @param[in]  funname  name of the last function called
 * @param[in]  ret      result of the last function called
 */
static void socket_layer_log_if_error(const char *funname, int8_t ret)
{
	if (SOCK_ERR_NO_ERROR != ret)
		LOG(SOCKET_DBG "ERROR: %s returned [%d]" EOL, funname, ret);
}


/* extern bool restart_require; */
/**
 * @brief Handles events from the winc socket API
 *
 * @param[in]  sock   socket for which an event has been generated
 * @param[in]  evt    event type, one of enumeration tenuSocketCallbackMsgType values
 * @param[in]  msg    event message body
 *
 * @note evt parameter is uint8_t and not tenuSocketCallbackMsgType
 * to match Atmel's callback signatures
 */
static void socket_layer_cb(SOCKET sock, uint8_t evt, void *msg)
{
	socket_desc *current_sock_desc = NULL;
	int i;
	for (i=0; i<NO_OF_SOCKETS; ++i)
	{
		if (sockets[i].sock == sock)
		{
			current_sock_desc = &sockets[i];
			break;
		}
	}
	if (current_sock_desc == NULL)
	{
		LOG(SOCKET_DBG "ERROR: CB called for invalid socket %d" EOL, (int) sock);
		return;
	}
	switch(evt)
	{
		case SOCKET_MSG_SENDTO:
		{
			int bytes_sent = *(int16_t *)msg;
			if (bytes_sent > 0)
			{
				/* winc1500_packet_sent(); */
				SOCKET_DEBUG_LOG(SOCKET_DBG "msg sent %db" EOL, bytes_sent);
			}
			else
			{
				SOCKET_DEBUG_LOG(SOCKET_DBG "msg sending failed !!! " EOL);
				/* Reset because of reinitalization socket problem */
				_reset_mcu();
			}
		}
		break;
		case SOCKET_MSG_BIND:
		{
			tstrSocketBindMsg *bind = (tstrSocketBindMsg*)msg;
			if (bind && bind->status == 0)
			{
				LOG(SOCKET_DBG "BIND ok" EOL);
				socket_layer_active = true;
				recvfrom(sock, socket_recv_buff, MTU, 0);
			} else {
				LOG(SOCKET_DBG "ERROR: bind!" EOL);
				// TODO: add error handling ?
			}
		}
		break;
		case SOCKET_MSG_RECVFROM:
		{
			tstrSocketRecvMsg *datagram = (tstrSocketRecvMsg *)msg;
			if (datagram)
			{
				uint8_t raddr[4]= {0};
				uint16_t rport = datagram->strRemoteAddr.sin_port;
				memcpy(raddr, &datagram->strRemoteAddr.sin_addr, sizeof(raddr));
				if (datagram->pu8Buffer && datagram->s16BufferSize > 1)
				{
					if (memcmp(&current_sock_desc->remote.sin_addr.s_addr,
					           &raddr[0],
					           sizeof(current_sock_desc->remote.sin_addr.s_addr)) == 0)
					/*
					 * INFO: server responses may come from any port
					 */
					{
                    	current_sock_desc->remote.sin_port = rport;

						LOG(SOCKET_DBG "INFO: received %d [B] from %u.%u.%u.%u:%u <========" EOL,
							datagram->s16BufferSize, raddr[0], raddr[1], raddr[2], raddr[3], _ntohs(rport));
						int16_t len = datagram->s16BufferSize;
						/* ASSERT(len >= 0); */

						if (current_sock_desc->cb)
							current_sock_desc->cb(&datagram->strRemoteAddr,
							                      (uint8_t *)datagram->pu8Buffer,
							                      len);
						if (datagram->u16RemainingSize)
						{
							LOG(SOCKET_DBG "WARNING: u16RemainingSize %u" EOL,datagram->u16RemainingSize);
						}
					}
					else
					{
						/* packet not from server */
						SOCKET_DEBUG_LOG(SOCKET_DBG "WARNING: datagram from %u.%u.%u.%u:%u length: %u" EOL,
							raddr[0], raddr[1], raddr[2], raddr[3], _ntohs(rport), datagram->s16BufferSize);
					}
				}
				else if (datagram->s16BufferSize == SOCK_ERR_TIMEOUT)
				{
					/* Prepare next buffer reception. */
					SOCKET_DEBUG_LOG(SOCKET_DBG "WARNING: recvfrom timeout" EOL);
				}
				else
				{
					SOCKET_DEBUG_LOG(SOCKET_DBG "ERROR: datagram from %u.%u.%u.%u:%u buff [%p] length: [%u]" EOL,
						raddr[0], raddr[1], raddr[2], raddr[3], _ntohs(rport),
						datagram->pu8Buffer, datagram->s16BufferSize);
				}
			}
			else
			{
				LOG(SOCKET_DBG "ERROR: empty datagram?" EOL);
			}
			recvfrom(sock, socket_recv_buff, MTU, 0);
		}
		break;
		default:
		{
			LOG(SOCKET_DBG "sock_cb: s:%d e:%u m:%p" EOL, sock, evt, msg);
		}
		break;
	}
}

/**
 * @brief
 *
 * @return
 */
static int get_socket_desc_index(SOCKET sock)
{
	int i;
	for (i=0; i<NO_OF_SOCKETS; ++i)
	{
		if (sockets[i].sock == sock)
		{
			return i;
		}
	}
	return -1;
}

/**
 * @brief Bind socket to address
 */
socket_desc *winc1500_socket_bind(socket_desc *sock,struct sockaddr_in *addr)
{
	if (sock->sock < 0)
	{
		return NULL;
	}

	sock->local.sin_family = addr->sin_family;
	sock->local.sin_addr.s_addr = addr->sin_addr.s_addr;
	if (addr->sin_port == 0)
	{
		sock->local.sin_port = BASE_LOCAL_PORT + get_socket_desc_index(sock->sock);
	}
	else
		sock->local.sin_port = addr->sin_port;
	int8_t ret = bind(sock->sock, (struct sockaddr *)&sock->local, sizeof(struct sockaddr_in));
	socket_layer_log_if_error("bind() %d",ret);
	LOG(SOCKET_DBG "socket = %d" EOL, sock->sock);

	return sock;
}

/**
 * @brief Connect socket to address
 */
socket_desc *winc1500_socket_connect(socket_desc *sock,struct sockaddr_in *addr)
{
	if (sock->sock < 0)
	{
		return NULL;
	}

	sock->remote.sin_family = addr->sin_family;
	sock->remote.sin_addr.s_addr = addr->sin_addr.s_addr;
	sock->remote.sin_port = addr->sin_port;
	LOG("socket:%d connect()", sock->sock);
	return sock;
}



/**
 * @brief Close socket
 */
int winc1500_socket_close(socket_desc *sock)
{
	if (sock->sock > 0)
	{
		LOG("winc1500_socket_close: %d\r\n",sock->sock);
		close(sock->sock);
		sock->sock = -1;
	}
	return 0;
}

/**
 * @brief Initializes socket
 */
socket_desc *winc1500_socket(uint16 u16Domain, uint8 u8Type, uint8 u8Flags)
{
	int i;

	for (i=0; i<NO_OF_SOCKETS; ++i)
	{
		if (sockets[i].sock == -1)
		{
			/* if ((sock->sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) */
			if ((sockets[i].sock = socket(u16Domain, u8Type, u8Flags)) < 0)
			{
				LOG(SOCKET_DBG "ERROR: failed to create UDP socket!");
				return NULL;
			}
			return &sockets[i];
		}
	}

	return NULL;
}

/**
 * @brief Initializes mcu sockets layer
 */
bool winc1500_socket_layer_init(void)
{
	int i;
	if (!cb_registered)
	{
		socketInit();
		registerSocketCallback(socket_layer_cb, NULL);
		cb_registered = true;
		for (i=0; i<NO_OF_SOCKETS; ++i)
			sockets[i].sock = -1;
	}

	/* if (!winc1500_is_wifi_connected()) */
		/* return false; */

	if (winc1500_is_socket_layer_active())
		return true;

	return false;
}

/**
 * @brief De-initialize sockets
 */
void winc1500_socket_layer_deinit(void)
{
	socket_layer_active = false;
	int i;
	for (i=0; i<NO_OF_SOCKETS; ++i)
	{
		if (sockets[i].sock >= 0)
		{
			close(sockets[i].sock);
			sockets[i].sock = -1;
		}
	}
}

/**
 * @brief Checks if sockets are ready to be used
 */
bool winc1500_is_socket_layer_active(void)
{
	return socket_layer_active;
}

/**
 * @brief Sends a packet via the UDP socket
 */
int16_t winc1500_udp_sendto(socket_desc *sock, struct sockaddr_in *addr, uint8_t *msg, uint16_t msg_len)
{
	int16_t ret = sendto(sock->sock,
	                    (void *)msg,
	                     msg_len,
	                     0,
	                     (struct sockaddr*)addr,
	                     sizeof(struct sockaddr_in));
	return ret;
}

/**
 * @brief Sends a packet via the UDP socket
 */
int16_t winc1500_udp_send(socket_desc *sock, uint8_t *msg, uint16_t msg_len)
{
	return winc1500_udp_sendto(sock, &sock->remote, msg, msg_len);
}

