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

#include <stdio.h>
#include <atmel_start.h>
#include "amp_agent.h"
#include "prox_agent_amp.h"
#include "prox_agent_config.h"

/**
 * Proximetry agent residing on the device is uniquely identified by device type (model) and software version.
 * It also has fixed lists of configuration parameters, statistics and alarms, which are reported to
 * and managed by the AirSync cloud.
 *
 * The definition of each type of device must be reflected in capability file located in the AirSync cloud.
 * Capability file is an XML file, which describes the Agent functionality in terms of statistics, parameters and alarms
 * supported by the specific device.
 * The name of capability file has the following form: metercap_<DEVICE_MODEL>_<CAPFILE_VER>.xml
 *
 * NOTE:
 * In case the user wants to add/remove configuration parameter, statistics or alarm,
 * the change should be made both in the agent code (see definitions below) and in the capability file.
 * The detailed description how to proceed is placed in the documentation for the Agent library.
 *
*/


static void (*param_changed)(uint32_t id);

#ifndef EOL
	#define EOL "\n\r"
#endif
#ifndef LOG
	/* #define LOG(...) */
	#define LOG(...) printf(__VA_ARGS__)
#endif
#ifndef ARRAY_ELEMS
    #define ARRAY_ELEMS(arr)  (sizeof(arr) / sizeof(arr[0]))
#endif


#define PROX_DEVICE_MODEL 	0x80168
#define PROX_CAPFILE_VER 	2

#define APP_VER   "0.0.1"

#if !defined(PROX_DEVICE_MODEL) || !defined (PROX_CAPFILE_VER)
	#error "DEVICE_MODEL and CAPFILE_VER must be defined"
#endif

/**
 * Design of the Agent assumes no dynamic memory allocation, so all STRING configuration parameters must be statically allocated by the user.
 * Non-string parameters, statistics and alarms are allocated in place during their definitions in struct amp_param_t params.
 * */

#define PARAM_STR_LEN 100
#define PARAM_1_STR_LEN PARAM_STR_LEN
#define PARAM_2_STR_LEN PARAM_STR_LEN

/* Statically allocated memory for agent string parameters */
static struct str_agent_params_s {
	char param_1[PARAM_1_STR_LEN];
	char param_2[PARAM_2_STR_LEN];
} str_agent_params;

/**
 * @brief Definitions of agent parameters
 *
 * To define a parameter, a param_id and proper AMP_AGENT_<TYPE>_PARAM_DEF macro should be used.
 * If the parameter type is STRING, additional statically allocated memory reference for this parameter must be given.
 * */
static amp_param_t params[] =
{
	AMP_AGENT_STRING_PARAM_DEF(PROX_AGENT_PARAM_ID_STR__1,str_agent_params.param_1),
	AMP_AGENT_STRING_PARAM_DEF(PROX_AGENT_PARAM_ID_STR__2,str_agent_params.param_2),
	AMP_AGENT_INT32_PARAM_DEF(PROX_AGENT_PARAM_ID_INT32__3),
	AMP_AGENT_INT32_PARAM_DEF(PROX_AGENT_PARAM_ID_INT32__4),
};

/**
 * @brief Definitions of agent statistics
 *
 * To define a statistic, a stat_id and proper AMP_AGENT_<TYPE>_STAT_DEF macro should be used.
 * */
static amp_stat_t stats[] =
{
	AMP_AGENT_INT32_STAT_DEF(PROX_AGENT_STAT_ID_INT32__1),
	AMP_AGENT_INT32_STAT_DEF(PROX_AGENT_STAT_ID_INT32__2),
	AMP_AGENT_FLOAT_STAT_DEF(PROX_AGENT_STAT_ID_FLOAT__3),
	AMP_AGENT_FLOAT_STAT_DEF(PROX_AGENT_STAT_ID_FLOAT__4),
};

/**
 * @brief Definitions of agent alarms
 *
 * To define an alarm, an alarm_id and AMP_AGENT_ALARM_DEF macro should be used.
 * */
static amp_alarm_t alarms[] =
{
	AMP_AGENT_ALARM_DEF(PROX_AGENT_ALARM_ID__1),
	AMP_AGENT_ALARM_DEF(PROX_AGENT_ALARM_ID__2)
};

/**
 * Internal storage for configuration parameters, statistics and alarms.
 *
 * */
static system_data_t agent_data =
{
	.params = params,
	.params_num = ARRAY_ELEMS(params),
	.stats = stats,
	.stats_num = ARRAY_ELEMS(stats),
	.alarms = alarms,
	.alarms_num = ARRAY_ELEMS(alarms)
};


/**
 * @brief System information utilized by the agent.
 * */
static system_info_t sys_info;
static device_api_t dev_api;


/**
 * @brief Agent change parameters indicator
 *
 * This function is invoked by the agent library if given parameter has been changed.
 *
 * Here should being put the code related to agent internal functionalities.
 * The user related parameters handling should being put in the prox_agent_param_changed function from the prox_agent_helper.c file.
 */
static void amp_agent_param_changed(amp_param_t *param)
{
	if (param_changed != NULL)
		param_changed(param->id);
}


/********************************************************************************************************************************/
/*                                       agent misc routines                                                                    */
/********************************************************************************************************************************/


/**
 * @brief Prints the agent parameters values - logging purpose
 */
void prox_agent_dump_prams(void)
{
	char param1[PARAM_1_STR_LEN];
	char param2[PARAM_2_STR_LEN];
	int32_t param3;
	int32_t param4;

	amp_agent_read_param_string(PROX_AGENT_PARAM_ID_STR__1,param1);
	amp_agent_read_param_string(PROX_AGENT_PARAM_ID_STR__2,param2);
	amp_agent_read_param_i32(PROX_AGENT_PARAM_ID_INT32__3,&param3);
	amp_agent_read_param_i32(PROX_AGENT_PARAM_ID_INT32__4,&param4);

	LOG("--- Agent params ---"EOL);
	LOG("   id:%4d param1 %s"EOL,PROX_AGENT_PARAM_ID_STR__1,param1);
	LOG("   id:%4d param2 %s"EOL,PROX_AGENT_PARAM_ID_STR__2,param2);
	LOG("   id:%4d param3: %d"EOL,PROX_AGENT_PARAM_ID_INT32__3,(int)param3);
	LOG("   id:%4d param4: %d"EOL,PROX_AGENT_PARAM_ID_INT32__4,(int)param4);
}

/**
 * @brief Prints agent parameters values - logging purpose
 */
void dump_sys_info(void)
{
	LOG("--- Agent system info ---"EOL);
	LOG("   Device id:         %s"EOL,sys_info.device_id);
	LOG("   Device name:       %s"EOL,sys_info.device_name);
	LOG("   Model id:          0x%X"EOL,(int)sys_info.model_id);
	LOG("   Software ver:      0x%X"EOL,(int)sys_info.software_ver);
	LOG("   Sync.msg.interval  %d"EOL,sys_info.sync_msg_interval);
	/* LOG("   Sync.msg.backoff  %d"EOL,sys_info.sync_msg_backoff); */
	LOG("   Activation code:   %s"EOL,sys_info.activation_code);
}

/**
 * @brief Agent init function
 *
 * - fills the agent library system info structure,
 * - initializes the agent library with sys_info and data structures (params,stats,alarms),
 * - updates the agent parameters values
 */
int prox_agent_amp_init(prox_agent_config_t *config, prox_agent_comm_cb_t *comm_cb)
{
	if (NULL == comm_cb->get_device_id) {
		LOG("get_device_id callback is required");
		return -1;
	}
	if (NULL == comm_cb->get_device_name) {
		LOG("get_device_name callback is required");
		return -1;
	}
	if (NULL == comm_cb->param_changed) {
		LOG("param_changed callback is required");
		return -1;
	}
	if (NULL == comm_cb->send_msg) {
		LOG("send_msg callback is required");
		return -1;
	}
	if (NULL == comm_cb->now) {
		LOG("now callback is required");
		return -1;
	}
	if (NULL == comm_cb->stats_update) {
		LOG("statu_update callback is empty");
	}

	/* fill the agent system info structure */
	sys_info.model_id = PROX_DEVICE_MODEL;
	sys_info.software_ver = PROX_CAPFILE_VER;
	sys_info.sync_msg_backoff = -1;
	sys_info.sync_msg_interval = config->sync_msg_interval;
	sys_info.activation_code = config->activation_code;

	sys_info.device_id = comm_cb->get_device_id();
	sys_info.device_name = comm_cb->get_device_name();

	/* dump_sys_info(); */

	param_changed = comm_cb->param_changed;

	dev_api.param_changed = amp_agent_param_changed;
	dev_api.send_msg = comm_cb->send_msg;
	dev_api.now = comm_cb->now;
	dev_api.stats_update = comm_cb->stats_update;

	amp_agent_set_log(AMP_LOG_INFO_E,PROX_AGENT_LOG_PREFIX);
	/* initialize agent library*/
	amp_agent_init(&sys_info, &agent_data, &dev_api);


	return 0;
}

