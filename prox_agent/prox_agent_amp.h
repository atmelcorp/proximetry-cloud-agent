
/*******************************************************************************
 *
 * Proximetry Cloud Agent AMP
 *
 * Copyright (c) 2015 Proximetry Inc. All rights reserved.
 * Authors: Proximetry
 * Copyright (c) 2015 Proximetry, Inc.
 * All Rights Reserved
 *
 ******************************************************************************/
/**
 * @file prox_agent_amp.h
 * @brief Proximety Cloud Agent AMP
 * */
#ifndef PROX_AGENT_AMP_H
#define PROX_AGENT_AMP_H

/* Parameters IDs */
#define PROX_AGENT_PARAM_ID_STR__1		1001
#define PROX_AGENT_PARAM_ID_STR__2		1002
#define PROX_AGENT_PARAM_ID_INT32__3	1003
#define PROX_AGENT_PARAM_ID_INT32__4	1004

/* Statistics IDs */
#define PROX_AGENT_STAT_ID_INT32__1		101
#define PROX_AGENT_STAT_ID_INT32__2		102
#define PROX_AGENT_STAT_ID_FLOAT__3		103
#define PROX_AGENT_STAT_ID_FLOAT__4		104

/* Alarms IDs */
#define PROX_AGENT_ALARM_ID__1		11
#define PROX_AGENT_ALARM_ID__2		12


#define PARAM_STR_LEN 100

#define PROX_AGENT_LOG_PREFIX "prox_agent: "
/**
 * @brief An agent initial configurtion structure
 */
typedef struct
{
	int sync_msg_interval; /**< @brief sync message interval [s] */
	const char *activation_code;   /**< @brief activation code */

}prox_agent_config_t;

/**
 * @brief An agnet initial communicatoin callbacks structure 
 */
typedef struct
{
	int (*send_msg)(uint8_t *data, uint16_t size);
	/**
	 * @brief 
	 */
	uint64_t (*now)(void);
	/**
	 * @brief Proximetry Agent change parameters indicator
	 *
	 * This function is invoked by the agent if given parameter has been changed.
	 * Here is the place for adding the logic/action when the parameters is changed.
	 */
	void (*param_changed)(uint32_t id);

	/**
	 * @brief Proximetry Agent set parameters function
	 *
	 * This function should contains the code which fill in the agent parameters with their current values. 
	 * The function is called during agent initialization, and may be considered as 
	 * parameter initial function. To set the parameters the prox_agent_amp.h api should being used.
	 * 
	 */
	void (*stats_update)(void);
	/**
	 * @brief Proximetry Agent set alarms function
	 *
	 * This function should contains the code which fill in the agent alarms with their current values. 
	 * The function is called periodically by agent.
	 * To set/clear the alarms the prox_agent_amp.h api should being used. 
	 */
	const char *(*get_device_name)(void);

	/**
	 * @brief Proximetry Agent get device name
	 *
	 * This function is utilizes by agent to get the device id.
	 * The device id should be unique for each device. The best candidate for the device_id is a mcu serial number. 
	 * Proximetry Device_ID is a max 100 chars width string.
	 */
	const char *(*get_device_id)(void);
} prox_agent_comm_cb_t;

/**
 * @brief Proximety Agent protocol initial function
 */
int prox_agent_amp_init(prox_agent_config_t *config, prox_agent_comm_cb_t *comm_cb);

/**
 * @brief Prints agent parameters values - logging purpose
 */
void prox_agent_dump_prams(void);


#ifndef __AMP_AGENT_H__
/* The api is inherited from amp_agent.h
 * ( a subsed copy of the libampagent api (amp_agent.h) )
 * The api should be used to read/write the parameters,statistics and alarmas values 
 * into agent instance.
 */

int amp_agent_write_param_i32(uint32_t id, int32_t value);
int amp_agent_write_param_string(uint32_t id, const char *value);

int amp_agent_read_param_i32(uint32_t id, int32_t *value);
int amp_agent_read_param_string(uint32_t id, char *value);

int amp_agent_write_stat_i32(uint32_t id, int8_t value);
int amp_agent_write_stat_float(uint32_t id, float value);


void amp_agent_set_alarm(uint32_t id);
void amp_agent_clear_alarm(uint32_t id);

/**
 * @brief Low level API function for handling incoming messages
 * @param[in]   msg     message buffer
 * @param[in]   size    message size
 */
void amp_agent_handle_msg(uint8_t *msg, uint16_t size);

/**
 * @brief Task for handling periodic sync message sending.
 * The function does nothing if sending time of next sync message hasn't been
 * reached. This function should be called as often as possible to avoid delays
 * in sending of sync messages.
 */
void amp_agent_sync_task(void);

/**
 * @brief Check if AMP Agent is connected to AS server
 * @return true if connected, false if not connected or uninitialised
 */
bool amp_agent_is_connected(void);

/**
 * @brief Notify AS about configuration parameter update by device
 */
void amp_agent_param_set(void);

#endif

#endif
