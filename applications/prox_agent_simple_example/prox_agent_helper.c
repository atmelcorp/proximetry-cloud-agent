/*******************************************************************************
 *
 * AMP Agent example implementation
 *
 * Copyright (c) 2015 Proximetry, Inc.
 * All Rights Reserved
 *
 ******************************************************************************/
/**
 *
 * @file prox_agent.c
 * @brief AMP Agent
 * */

#include <math.h>
#include <atmel_start.h>
#include "driver/include/m2m_wifi.h"
#include "prox_agent.h"
#include "prox_agent_common.h"
#include "prox_agent_amp.h"
#include "prox_agent_helper.h"
#include "prox_agent_config.h"
#include "at30tse75x.h"
#include "main.h"
#include <temperature_sensor_main.h>

/**
 * @brief Agent change parameters indicator
 *
 * This routine is invoked by the agent library if given parameter has been changed.
 * Here is the place for adding the logic/action when the parameters is changed.
 */
void prox_agent_param_changed(uint32_t param_id)
{
	int32_t value;

	if (param_id == PROX_AGENT_PARAM_ID_INT32__3)    /* LED0 */
	{
		amp_agent_read_param_i32(PROX_AGENT_PARAM_ID_INT32__3,&value);
		LOG("%s has changed (LED0): %d"EOL,STR(PROX_AGENT_PARAM_ID_INT32__3),(int)value);
		// Set LED0
		if (0 == value )
			led0_off();      /* disable LED0 */
		else
			led0_on();       /* enable LED0  */
	}
	else if (param_id == PROX_AGENT_PARAM_ID_INT32__4) /* LED0 blinking freq */
	{
		amp_agent_read_param_i32(PROX_AGENT_PARAM_ID_INT32__4,&value);
		LOG("%s has changed: (LED0 freq) %d"EOL,STR(PROX_AGENT_PARAM_ID_INT32__4),(int)value);
		set_led0_blinking_freq(value);
	}
}

/**
 * @brief Set statistic in agent
 *
 * This routine is invoked by the agent library
 */
void prox_agent_stats_update(void)
{
	int32_t stat1 = 0;
	int32_t stat2 = 0;
	float stat3 = 0;
	float stat4 = 0;

	stat1 = io1_read_illuminance_blk();
	stat2 = gpio_get_pin_level(SW0);
	stat3 = temperature_sensor_read(AT30TSE75X);
	stat4 = (float)wifi_get_rssi_blk();

	amp_agent_write_stat_i32(PROX_AGENT_STAT_ID_INT32__1, stat1);
	amp_agent_write_stat_i32(PROX_AGENT_STAT_ID_INT32__2, stat2);
	amp_agent_write_stat_float(PROX_AGENT_STAT_ID_FLOAT__3, stat3);
	amp_agent_write_stat_float(PROX_AGENT_STAT_ID_FLOAT__4, stat4);

	LOG( "PROX_AGENT_STAT_ID_INT32__1: %d" EOL,(int)stat1);
	LOG( "PROX_AGENT_STAT_ID_INT32__2: %d" EOL,(int)stat2);
	LOG( "PROX_AGENT_STAT_ID_FLOAT__3: " FLOAT_TO_INT_POINT_INT_FORMAT EOL,FLOAT_TO_INT_POINT_INT_VALUES(stat3));
	LOG( "PROX_AGENT_STAT_ID_FLOAT__4: " FLOAT_TO_INT_POINT_INT_FORMAT EOL,FLOAT_TO_INT_POINT_INT_VALUES(stat4));
}

/**
 * @brief Sets parameter values in agent
 *
 * Fills the agent parameters structures based on the system information.
 * L3 related parameters are updated by prox_agent_update_network_param().
 */
int prox_agent_set_params(void)
{
	amp_agent_write_param_string(PROX_AGENT_PARAM_ID_STR__1,get_mac_str());
	amp_agent_write_param_string(PROX_AGENT_PARAM_ID_STR__2,get_ip_str());
	amp_agent_write_param_i32(PROX_AGENT_PARAM_ID_INT32__3,get_led0_service_state());
	amp_agent_write_param_i32(PROX_AGENT_PARAM_ID_INT32__4,get_led0_freq());

	amp_agent_param_set();

	return 0;
}


/**
 * @brief Sets network parameter in cloud agent
 *
 * This function should be called from a dhcp callback when the ip settings are valid
 */
int prox_agent_update_network_param(void)
{
	amp_agent_write_param_string(PROX_AGENT_PARAM_ID_STR__2,get_ip_str());

	return 0;
}


/**
 * @brief Updates alarm states in agent
 *
 * This routine is invoked by the agent thread for updating the alarm states
 * based on the system state.
 */
void prox_agent_alarms_update(void)
{
	static bool alarm1 = false;
	static bool alarm2 = false;

#define TEMPERATURE_ALERT_ON_LEVEL    29.0
#define TEMPERATURE_ALERT_OFF_LEVEL   28.0
    //temperature alarm with hysteresis
	if( TEMPERATURE_ALERT_ON_LEVEL < temperature_sensor_read(AT30TSE75X)){
		if (alarm1 == false)
			LOG("alert on: Temperature > " FLOAT_TO_INT_POINT_INT_FORMAT EOL,FLOAT_TO_INT_POINT_INT_VALUES(TEMPERATURE_ALERT_ON_LEVEL));
		alarm1 = true;
			amp_agent_set_alarm(PROX_AGENT_ALARM_ID__1);
	}else if (TEMPERATURE_ALERT_OFF_LEVEL > temperature_sensor_read(AT30TSE75X)){
		if (alarm1 == true)
			LOG("alert off: Temperature < " FLOAT_TO_INT_POINT_INT_FORMAT EOL,FLOAT_TO_INT_POINT_INT_VALUES(TEMPERATURE_ALERT_OFF_LEVEL));
		alarm1 = false;
			amp_agent_clear_alarm(PROX_AGENT_ALARM_ID__1);
		}

	// SW0 alarm
    if (0 == gpio_get_pin_level(SW0)){
		if (alarm2 == false)
			LOG("alert on: SW0"EOL);
		alarm2 = true;
			amp_agent_set_alarm(PROX_AGENT_ALARM_ID__2);
		}else {
		if (alarm2 == true)
			LOG("alert off: SW0"EOL);
		alarm2 = false;
			amp_agent_clear_alarm(PROX_AGENT_ALARM_ID__2);
		}
}

/**
 * @brief Proximetry Agent get device name
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
 * @brief Proximetry Agent get device name
 *
 * This function is utilizes by agent to get the device name which will be reported to cloud server.
 * Proximetry Device Name is a max 32 chars width string;
 */
const char *prox_get_device_name(void)
{
	static char device_name[100];
	uint8_t mac[6];
	if (m2m_wifi_get_mac_address(mac) != M2M_SUCCESS)
	{
		error_stop("Unable to get wifi mac address");
	}
	sprintf(device_name,"%s_%02X%02X%02X",PROX_DEVICE_NAME,mac[3],mac[4],mac[5]);
	return device_name;
}
