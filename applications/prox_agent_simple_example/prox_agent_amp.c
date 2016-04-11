/*******************************************************************************
 *
 * Proximetry AMP Agent Application
 *
 * Copyright (c) 2016 Proximetry Inc. All rights reserved.
 * Authors: Proximetry
 *
 ******************************************************************************/

/**
 * @file prox_agent_amp.c
 * @brief AMP Agent Application
 */

#include <stdio.h>
#include "prox_agent_amp.h"
#include "prox_agent.h"
#include "prox_agent_config.h"
#include "prox_agent_common.h"
#include <temperature_sensor_main.h>

/**
 * Capabilities of each Proximetry Agent Application is uniquely identified by a device model (PROX_DEVICE_MODEL) and capabilities version (PROX_CAPFILE_VER).
 * The definition of an application must be know to the Cloud Service before a first device running that application registers to the Cloud.
 * Application capabilities are described in a Capability File. Capability File describes application functionality in terms of supported configuration parameters, statistics and alarms.
 * Each application must implement all data types defined in the corresponding Capability File.
 * Please refer to the Agent Library documentation for more information about the Proximetry Agent Application development.
 *
 * NOTE:
 * In case the developer wants to add/remove configuration parameter, statistics or alarm,
 * the change should be made both in the application code and in the corresponding Capability File.
 */

#ifndef EOL
    #define EOL "\n\r"
#endif
#ifndef LOG
    #define LOG(...) printf(__VA_ARGS__)
#endif
#ifndef ARRAY_ELEMS
    #define ARRAY_ELEMS(arr)  (sizeof(arr) / sizeof(arr[0]))
#endif
#define PROX_DEVICE_MODEL  0x80168
#define PROX_CAPFILE_VER   4
#define APP_VER   "0.0.2"

/**
 * This application does not use dynamic memory allocation, all STRING configuration parameters must be statically allocated below.
 * Non-string parameters, statistics and alarms are allocated in place during their definitions in struct amp_param_t params.
 */

/* Statically allocated memory for string parameters */
static struct str_agent_params_s
{
  char str_mac[PARAM_STR_LEN];
  char str_ip[PARAM_STR_LEN];

} str_agent_params;

/**
 * @brief Definitions of configuration parameters
 *
 * To define a configuration parameter, a param_id and proper AMP_AGENT_<TYPE>_PARAM_DEF macro should be used.
 * For STRING parameters the statically allocated memory reference must be provided.
 * */
static amp_param_t params[] =
{
    AMP_AGENT_STRING_PARAM_DEF(PROX_AGENT_PARAM_ID_STR_MAC,str_agent_params.str_mac),
    AMP_AGENT_STRING_PARAM_DEF(PROX_AGENT_PARAM_ID_STR_IP,str_agent_params.str_ip),
    AMP_AGENT_INT32_PARAM_DEF(PROX_AGENT_PARAM_ID_INT32_LED_ONOFF),
    AMP_AGENT_INT32_PARAM_DEF(PROX_AGENT_PARAM_ID_INT32_LED_FREQ)
};

/**
 * @brief Definitions of statistics
 *
 * To define a statistic, a stat_id and proper AMP_AGENT_<TYPE>_STAT_DEF macro should be used.
 * */
static amp_stat_t stats[] =
{
    AMP_AGENT_INT32_STAT_DEF(PROX_AGENT_STAT_ID_IO1_ILUM),
    AMP_AGENT_INT32_STAT_DEF(PROX_AGENT_STAT_ID_SW0),
    AMP_AGENT_FLOAT_STAT_DEF(PROX_AGENT_STAT_ID_IO1_TEMP),
    AMP_AGENT_FLOAT_STAT_DEF(PROX_AGENT_STAT_ID_WINC_RSSI)
};

/**
 * @brief Definitions of alarms
 *
 * To define an alarm, an alarm_id and AMP_AGENT_ALARM_DEF macro should be used.
 * */
static amp_alarm_t alarms[] =
{
    AMP_AGENT_ALARM_DEF(PROX_AGENT_ALARM_ID_IO1_TEMP),
    AMP_AGENT_ALARM_DEF(PROX_AGENT_ALARM_ID_SW0)
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
 * @brief System information utilized by the Proximetry Agent Application.
 * */
static system_info_t sys_info;
static device_api_t dev_api;

/**
 * @brief Parameter changed callback function
 *
 * This function is invoked by the Proximetry Agent Library, as a callback, when a configuration parameter has been changed from the Cloud service.
 * Here is the place for adding the logic related to the configuration parameter modification .
 */
static void prox_agent_param_changed(amp_param_t *param)
{
    int32_t value;

    switch(param->id)
    {
        case PROX_AGENT_PARAM_ID_INT32_LED_ONOFF:
            amp_agent_read_param_i32(param->id,&value);
            LOG("%s has changed (LED0): %d"EOL,STR(param->id),(int)value);
            // Set LED0
            if (0 == value )
                led0_off();      /* disable LED0 */
            else
                led0_on();       /* enable LED0  */
            break;
        case PROX_AGENT_PARAM_ID_INT32_LED_FREQ:
            amp_agent_read_param_i32(param->id,&value);
            LOG("%s has changed: (LED0 freq) %d"EOL,STR(param->id),(int)value);
            set_led0_blinking_freq(value);
            break;
  }
}

/**
 * @brief Update statistic values in the Proximetry Agent Library
 *
 * This routine is invoked periodically by the Proximetry Agent Library to send statistics to the Cloud
 * Put here code responsible for sensor readings
 */
static void prox_agent_stats_update(void)
{
    int32_t stat_io1_ilum;
    int32_t stat_sw0;
    float stat_io1_temp;
    float stat_winc_rssi;

    stat_io1_ilum = io1_read_illuminance_blk();
    stat_sw0 = gpio_get_pin_level(SW0) ? 0 : 1;
    stat_io1_temp = temperature_sensor_read(AT30TSE75X);
    stat_winc_rssi = (float)wifi_get_rssi_blk();

    //Write statistic values to the Proximetry Agent Library
    amp_agent_write_stat_i32(PROX_AGENT_STAT_ID_IO1_ILUM, stat_io1_ilum);
    amp_agent_write_stat_i32(PROX_AGENT_STAT_ID_SW0, stat_sw0);
    amp_agent_write_stat_float(PROX_AGENT_STAT_ID_IO1_TEMP, stat_io1_temp);
    amp_agent_write_stat_float(PROX_AGENT_STAT_ID_WINC_RSSI, stat_winc_rssi);

    LOG( "PROX_AGENT_STAT_ID_IO1_ILUM: %d" EOL, (int)stat_io1_ilum);
    LOG( "PROX_AGENT_STAT_ID_SW0: %d" EOL, (int)stat_sw0);
    LOG( "PROX_AGENT_STAT_ID_IO1_TEMP:" FLOAT_TO_INT_POINT_INT_FORMAT EOL, FLOAT_TO_INT_POINT_INT_VALUES(stat_io1_temp));
    LOG( "PROX_AGENT_STAT_ID_WINC_RSSI:" FLOAT_TO_INT_POINT_INT_FORMAT EOL, FLOAT_TO_INT_POINT_INT_VALUES(stat_winc_rssi));
}

/**
 * @brief Set parameter values in the Proximetry Agent Library
 *
 * Put here code responsible for reading device configuration parameters
 */
static int prox_agent_set_params(void)
{

    amp_agent_write_param_string(PROX_AGENT_PARAM_ID_STR_MAC,get_mac_str());
    amp_agent_write_param_string(PROX_AGENT_PARAM_ID_STR_IP,get_ip_str());
    amp_agent_write_param_i32(PROX_AGENT_PARAM_ID_INT32_LED_ONOFF,get_led0_service_state());
    amp_agent_write_param_i32(PROX_AGENT_PARAM_ID_INT32_LED_FREQ,get_led0_freq());

    amp_agent_param_set();

    return 0;
}

/**
 * @brief Update alarm states in the Proximetry Agent Library
 *
 * This routine is invoked by the Agent thread for updating alarm states.
 * Add here code responsible for setting and clearing alarms.
 */
static void prox_agent_alarms_update(void)
{

#define ALARM_OFF  false
#define ALARM_ON   true
#define TEMPERATURE_ALERT_ON_LEVEL    29.0
#define TEMPERATURE_ALERT_OFF_LEVEL   28.0

    static bool alert1 = ALARM_OFF;
    static bool alert2 = ALARM_OFF;

    float temp = temperature_sensor_read(AT30TSE75X);

    //temperature alarm with hysteresis
    if( temp > TEMPERATURE_ALERT_ON_LEVEL && alert1 == ALARM_OFF){
        LOG("alert1 on: Temperature > " FLOAT_TO_INT_POINT_INT_FORMAT EOL,FLOAT_TO_INT_POINT_INT_VALUES(TEMPERATURE_ALERT_ON_LEVEL));
        amp_agent_set_alarm(PROX_AGENT_ALARM_ID_IO1_TEMP);
        alert1 = ALARM_ON;
    }else if (temp < TEMPERATURE_ALERT_OFF_LEVEL && alert1 == ALARM_ON ){
        LOG("alert1 off: Temperature < " FLOAT_TO_INT_POINT_INT_FORMAT EOL,FLOAT_TO_INT_POINT_INT_VALUES(TEMPERATURE_ALERT_OFF_LEVEL));
        amp_agent_clear_alarm(PROX_AGENT_ALARM_ID_IO1_TEMP);
        alert1 = ALARM_OFF;
    }

    // SW0 alarm
    if (0 == gpio_get_pin_level(SW0) && alert2 == ALARM_OFF){
        LOG("alert2 on: SW0"EOL);
        amp_agent_set_alarm(PROX_AGENT_ALARM_ID_SW0);
        alert2 = ALARM_ON;
    }else if (1 == gpio_get_pin_level(SW0) && alert2 == ALARM_ON){
        LOG("alert2 off: SW0"EOL);
        amp_agent_clear_alarm(PROX_AGENT_ALARM_ID_SW0);
        alert2 = ALARM_OFF;
    }
}

/********************************************************************************************************************************/
/*                                       agent misc routines                                                                    */
/********************************************************************************************************************************/

/**
 * @brief Prints the Proximetry Agent Application configuration parameters
 */
static void prox_agent_dump_prams(void)
{
    char param_str_mac[PARAM_STR_LEN] = {0};
    char param_str_ip[PARAM_STR_LEN] = {0};
    int32_t param_int32_led_onoff;
    int32_t param_int32_led_freq;

    //Read param values from the Proximetry Agent Library
    amp_agent_read_param_string(PROX_AGENT_PARAM_ID_STR_MAC, param_str_mac);
    amp_agent_read_param_string(PROX_AGENT_PARAM_ID_STR_IP, param_str_ip);
    amp_agent_read_param_i32(PROX_AGENT_PARAM_ID_INT32_LED_ONOFF, &param_int32_led_onoff);
    amp_agent_read_param_i32(PROX_AGENT_PARAM_ID_INT32_LED_FREQ, &param_int32_led_freq);

    LOG( "PROX_AGENT_PARAM_ID_STR_MAC: %s" EOL, param_str_mac);
    LOG( "PROX_AGENT_PARAM_ID_STR_IP: %s" EOL, param_str_ip);
    LOG( "PROX_AGENT_PARAM_ID_INT32_LED_ONOFF: %d" EOL, (int)param_int32_led_onoff);
    LOG( "PROX_AGENT_PARAM_ID_INT32_LED_FREQ: %d" EOL, (int)param_int32_led_freq);
}

/**
 * @brief Prints the Proximetry Agent Library configuration parameters
 */
static void dump_sys_info(void)
{
    LOG("--- Agent system info ---" EOL);
    LOG("   Device id:         %s" EOL, sys_info.device_id);
    LOG("   Device name:       %s" EOL, sys_info.device_name);
    LOG("   Model id:          0x%X" EOL, (int)sys_info.model_id);
    LOG("   Software ver:      0x%X" EOL, (int)sys_info.software_ver);
    LOG("   Sync.msg.interval  %d" EOL, sys_info.sync_msg_interval);
    LOG("   Activation code:   %s" EOL, sys_info.activation_code);
}

/**
 * @brief Proximetry Agent Application init function
 *
 * - fills the Proximetry Agent Library system info structure,
 * - initializes the Proximetry Agent Library with sys_info and data structures (params,stats,alarms),
 * - initializes the Proximetry Agent Application parameters
 */
int prox_agent_init(void)
{
    LOG("agent_start" EOL);
    LOG("Server cloud IP: %s" EOL, PROX_SERVER_IP);

    /* fill the agent system info structure */
    sys_info.model_id          = PROX_DEVICE_MODEL;
    sys_info.software_ver      = PROX_CAPFILE_VER;
    sys_info.sync_msg_backoff  = -1;
    sys_info.sync_msg_interval = prox_get_sync_interval();
    sys_info.activation_code   = prox_get_activation_code();
    sys_info.device_id         = prox_get_device_id();
    sys_info.device_name       = prox_get_device_name();

    dump_sys_info();

    dev_api.param_changed      = prox_agent_param_changed;
    dev_api.send_msg           = prox_agent_send_msg;
    dev_api.now                = prox_agent_time_ms;
    dev_api.stats_update       = prox_agent_stats_update;

    amp_agent_set_log(AMP_LOG_INFO_E, PROX_AGENT_LOG_PREFIX);

    /* initialize Proximetry Agent Library*/
    amp_agent_init(&sys_info, &agent_data, &dev_api);

    /* initialize the Proximetry Agent Application parameters*/
    prox_agent_set_params();
    prox_agent_dump_prams();

    return 0;
}

/**
 * @brief Proximetry Agent Application non-blocking service routine
 *
 * - updates alarms states in the Proximetry Agent Library,
 * - performs the Agent functionality,
 *
 * It should be periodically invoked by the main task, at least once per sync message interval.
 */
void prox_agent_thread_nb(void)
{
    prox_agent_alarms_update();
    amp_agent_sync_task();
}
