/*******************************************************************************
 *
 * Proximetry AMP Agent Application
 *
 * Copyright (c) 2016 Proximetry Inc. All rights reserved.
 * Authors: Proximetry
 *
 ******************************************************************************/

/**
 * @file prox_agent.c
 * @brief AMP Agent
 * */

#include <math.h>
#include <stdlib.h>
#include "socket_layer.h"
#include "prox_agent.h"
#include "prox_agent_amp.h"
#include "prox_agent_config.h"
#include "prox_agent_common.h"
#include "driver/include/m2m_wifi.h"

static socket_desc *agent_sock = NULL;


/**
 * \brief function gives time stamp
 */
uint64_t prox_agent_time_ms(void)
{
	return time_ms();
}

/**
 * @brief Send message routine
 *
 * This routine is invoked by agent library
 * Agent utilizes this routine to send message
 */
int prox_agent_send_msg(uint8_t *data, uint16_t size)
{
	LOG("%s: sin_port:%d"EOL,__FUNCTION__,_htons(agent_sock->remote.sin_port));
	return winc1500_udp_send(agent_sock,(uint8_t*)data ,(uint16_t)size);
}

/**
 * @brief Receive message routine
 *
 * This routine is invoked by the system if any data for the agent is received
 */
static void prox_agent_recv_msg(struct sockaddr_in *src, uint8_t *buffer, size_t len)
{
	UNUSED(src);
	amp_agent_handle_msg((uint8_t*)buffer,len);
}

/**
 * @brief Agent socket init routine
 */
int  prox_init_udp_socket(void)
{
	/* allocate network resources */
	struct sockaddr_in local;
	struct sockaddr_in remote;

	LOG("Init agent socket"EOL);

	if(agent_sock != NULL)
	{
		winc1500_socket_close(agent_sock);
	}
	agent_sock = NULL;

	agent_sock = winc1500_socket(AF_INET, SOCK_DGRAM, 0);
	if (agent_sock == NULL)
	{
		printf("Agent socket error"EOL);
		return -1;
	}

	local.sin_family = AF_INET;
	local.sin_addr.s_addr = 0;
	local.sin_port =  0;

	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = read_ip(PROX_SERVER_IP);
	remote.sin_port =  _htons(PROX_PORT);

	winc1500_socket_bind(agent_sock,&local);
	winc1500_socket_connect(agent_sock,&remote);
	winc1500_socket_layer_register_recv_cb(agent_sock, prox_agent_recv_msg);

	return 0;
}

/**
 * @brief Proximetry Agent get device id
 *
 * This function is utilizes by agent to get the device id.
 * The device id should be unique for each device. The best candidate for the device_id is a mcu serial number.
 * Proximetry Device_ID is a max 100 chars width string.
 */
const char *prox_get_device_id(void)
{
    static char device_serial_no[33];

	uint32_t serial_no[4];

	serial_no[0] = *(uint32_t *)0x0080A00C;
	serial_no[1] = *(uint32_t *)0x0080A040;
	serial_no[2] = *(uint32_t *)0x0080A044;
	serial_no[3] = *(uint32_t *)0x0080A048;
	sprintf(device_serial_no, "%08x%08x%08x%08x",
		(unsigned)serial_no[0], (unsigned)serial_no[1],
		(unsigned)serial_no[2], (unsigned)serial_no[3]);
	device_serial_no[32] = '\0';

	return device_serial_no;
}

/**
 * @brief Proximetry Agent get a device name
 *
 * This function is utilizes by agent to get the device name which will be reported to cloud server.
 * Proximetry Device Name is a max 32 chars width string;
 */
const char *prox_get_device_name(void)
{
	return PROX_DEVICE_NAME;
}

/**
 * @brief Proximetry Agent get an activation code
 *
 */
const char *prox_get_activation_code(void)
{
	return PROX_ACTIVATION_CODE;
}

/**
 * @brief Proximetry Agent get a synchronization interval value
 *
 */
const unsigned int prox_get_sync_interval(void)
{
	return PROX_SYNC_INTERVAL;
}

