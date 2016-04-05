/*******************************************************************************
 *
 * AMP Agent example implementation
 *
 * Copyright (c) 2015 Proximetry, Inc.
 * All Rights Reserved
 *
 ******************************************************************************/

/**
 * @file prox_agent.c
 * @brief AMP Agent
 * */

#include <stdlib.h>
#include <atmel_start.h>
#include "prox_agent_common.h"
#include "socket_layer.h"
#include "prox_agent.h"
#include "prox_agent_amp.h"
#include "prox_agent_helper.h"
#include "prox_agent_config.h"
#include "prox_agent_common.h"
#include "main.h"

static socket_desc *agent_sock = NULL;

/**
 * @brief Send message routine
 *
 * This routine is invoked by agent library
 * Agent utilizes this routine to send message
 */
static int prox_agent_send_msg(uint8_t *data, uint16_t size)
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

		printf("Agent socke error"EOL);
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

/********************************************************************************************************************************/
/*                                       agent main routines                                                                    */
/********************************************************************************************************************************/

/**
 * @brief Agent init routine
 *
 * - fills the agent library system info structure,
 * - initializes the agent with sys_info and data structures (params,stats,alarms),
 * - set the agent parameters values
 */
void prox_cloud_agent_init(void)
{
	LOG("agent_start" EOL);
	LOG("Server cloud IP: %s"EOL,PROX_SERVER_IP);

	prox_agent_config_t config;
	config.sync_msg_interval = PROX_SYNC_INTERVAL;
	config.activation_code = PROX_ACTIVATION_CODE;

	prox_agent_comm_cb_t comm_cb;
	comm_cb.send_msg = prox_agent_send_msg;
	comm_cb.now 			= dev_now;
	comm_cb.param_changed 	= prox_agent_param_changed;
	comm_cb.stats_update 	= prox_agent_stats_update;
	comm_cb.get_device_name = prox_get_device_name;
	comm_cb.get_device_id 	= prox_get_device_id;

	prox_agent_amp_init(&config, &comm_cb);

	/* update the agent parameters values */
	prox_agent_set_params();
	prox_agent_dump_prams();
}

/**
 * @brief Agent non-blocking service routine
 *
 * - update alarms states in agent,
 * - performs the agent functionality,
 *
 * It should be periodically invoked by the main task, at least once per set sync message interval.
 */
void prox_cloud_agent_thread_nb(void)
{
	prox_agent_alarms_update();
	amp_agent_sync_task();

}
