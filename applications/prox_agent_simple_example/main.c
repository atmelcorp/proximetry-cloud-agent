#include <string.h>
#include <stdlib.h>

#include <atmel_start.h>
#include <winc1500_main.h>
#include <stdio_start.h>

#include <socket/include/socket.h>

#include <driver/include/m2m_wifi.h>
#include <at30tse75x.h>
#include "main.h"
#include <temperature_sensor_main.h>

#include <core/prox_core.h>
#include <prox_agent.h>
#include <helpers/prox_helpers.h>

#define CONNECTION_TIMEOUT   	10000  						//[ms] - timeout for wifi [re]connection

#define LOG printf

#define APP_VER "0.2.0"
#define STRING_HEADER EOL EOL "Start Proximetry Agent Simple Example " APP_VER " --" EOL \

#define EOL "\r\n"

/** Wi-Fi status variable. */
static bool gbConnectedWifi = false;
static int8_t wifi_rssi;
static char wifi_ip_str[16];

/* timer 0 - timestamp variables */
static struct timer_task TIMER_0_task1;
static uint64_t timer_1ms = 0;
static uint64_t connect_timer = 0;

/* led0 service variables */
static int32_t led0_blinking_freq = 1;
static bool led0 = true;

#define MTU			516
static uint8_t socket_recv_buff[MTU] = {0};

/**
 * \brief Gets Wi-Fi card MAC address
 *
 * \return MAC address
 */
const char *get_mac_str(void)
{
	static char mac_str[19];
	uint8_t mac[6];
	if (m2m_wifi_get_mac_address(mac) != M2M_SUCCESS)
	{
		printf("error: Unable to get wifi mac address");
		while(1)
			;
	}
	sprintf(mac_str,"%02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	return mac_str;
}

/**
 * \brief Get Wi-Fi IP address
 *
 * \return IP address
 */
const char *get_ip_str(void)
{
	return wifi_ip_str;
}

/**
 * \brief Get Wi-Fi connection status
 */
bool wifi_connected(void)
{
	return gbConnectedWifi;
}

/**
 * \brief Callback to get the Wi-Fi status update.
 *
 * \param[in] u8MsgType Type of Wi-Fi notification.
 * \param[in] pvMsg A pointer to a buffer containing the notification parameters.
 *
 * \return None.
 */
static void wifi_cb(uint8 u8MsgType, void *pvMsg)
{

	printf("--- wifi_cb ---\r\n");

	switch (u8MsgType) {
		case M2M_WIFI_RESP_CON_STATE_CHANGED:
		{
			tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
			if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {
				printf("Wi-Fi connected\r\n");
				m2m_wifi_request_dhcp_client();
			} else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {
				printf("Wi-Fi disconnected\r\n");
				gbConnectedWifi = false;
			}
			break;
		}

		case M2M_WIFI_REQ_DHCP_CONF:
		{
			uint8 *pu8IPAddress = (uint8 *)pvMsg;
			sprintf(wifi_ip_str,"%u.%u.%u.%u", pu8IPAddress[0], pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3]);
			printf("Wi-Fi IP is %s\r\n",wifi_ip_str);
			gbConnectedWifi = true;
/******************** PROX: Initialize Proximety Agent Socket *****************************************************/
			prox_agent_get_socket(true);
/******************************************************************************************************************/

/******************** PROX: Push the IP address obtained from DHCP to the Proximetry Library **********************/
			prox_conf_param_push__ip(wifi_ip_str);
/******************************************************************************************************************/
			break;
		}

		case M2M_WIFI_RESP_PROVISION_INFO:
		{
			tstrM2MProvisionInfo *pstrProvInfo = (tstrM2MProvisionInfo *)pvMsg;
			printf("wifi_cb: M2M_WIFI_RESP_PROVISION_INFO.\r\n");

			if (pstrProvInfo->u8Status == M2M_SUCCESS) {
				m2m_wifi_connect((char *)pstrProvInfo->au8SSID, strlen((char *)pstrProvInfo->au8SSID), pstrProvInfo->u8SecType,
						pstrProvInfo->au8Password, M2M_WIFI_CH_ALL);
			} else {
				printf("wifi_cb: Provision failed.\r\n");
			}
		}
		break;
		case M2M_WIFI_RESP_CURRENT_RSSI:
				wifi_rssi  = *(int8_t *)pvMsg;
				printf("wifi_cb: rssi: %d" EOL, wifi_rssi);
		break;

		default:
		{
			break;
		}
	}
}


/**
 * \brief Timer 0 task callback
 */
static void TIMER_0_task1_cb(const struct timer_task *const timer_task)
{
	timer_1ms++;
}

/**
 * \brief Get time
 *
 * \return timestamp in milliseconds
 *
 */
uint64_t time_ms(void)
{
	return timer_1ms;
}

/************ PROX: Callback used by the Proximetry Library to get timestamp **************************************/
/**
 * \brief Function returns timestamp in milliseconds
 */
uint64_t prox_callback__get_time_ms(void)
{
	return time_ms();
}
/******************************************************************************************************************/

/**
 * \brief Timer 0 initialization
 */
static void TIMER_0_init(void)
{
	TIMER_0_task1.interval = 1;
	TIMER_0_task1.cb = TIMER_0_task1_cb;
	TIMER_0_task1.mode = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task1);
	timer_start(&TIMER_0);
}

/**
 * \brief Set the blinking frequency for LED0
 */
int set_led0_blinking_freq(int32_t f)
{

	led0_blinking_freq = f;

	return 0;
}

/**
 * \brief Obtain the state of LED0 (on/off)
 */
bool get_led0_service_state(void)
{
	return led0;
}

/**
 * \brief Obtain the blinking frequency of LED0
 */
int32_t get_led0_freq(void)
{
	return led0_blinking_freq;
}

/**
 * \brief Turn LED0 on
 */
int led0_on(void)
{
	led0 = true;
	return 0;
}

/**
 * \brief Turn LED0 off
 */
int led0_off(void)
{
	led0 = false;
	return 0;
}

/**
 * \brief Blinking with led0_blinking_freq frequency
 *
 *  if led0_blinking_freq < 0 then
 * 	  	freq = abs(led0_blinking_freq)^(-1)
 */
void led0_service(void)
{
	static uint64_t next_blink = 0;

	if (false == led0){ 				/* led0 off */
		gpio_set_pin_level(LED0,1);
	}else{ 								/* led0 on */
		if (next_blink < time_ms()){
			/* positive freq */
			if (led0_blinking_freq > 0)
				next_blink = time_ms() + (1000/led0_blinking_freq)/2;
			/* negative freq */
			else if (led0_blinking_freq < 0)
				next_blink = time_ms() + (-led0_blinking_freq) * 1000/2;

			gpio_toggle_pin_level(LED0);
		}
	}
}

/**
 * \brief Connect wifi
 */
int connect_wifi(void)
{
	/* Connect to wifi */
	printf ("Connecting to wifi: SSID: %s"EOL,DEF_SSID);
	m2m_wifi_connect(DEF_SSID, strlen(DEF_SSID), DEF_SEC_TYPE, DEF_PASSWORD, M2M_WIFI_CH_ALL);
	return 0;
}

/**
 * \brief Gets RSSI value from the Wi-Fi driver using a blocking request
 *
 * \return RSSI value
 */
int8_t wifi_get_rssi_blk(void)
{
	if (!wifi_connected())
		return -1;

	m2m_wifi_req_curr_rssi();

	do
	{
		if (!wifi_connected())
			return -90;

		m2m_wifi_handle_events(NULL);
		delay_ms(1);

	}while (wifi_rssi == 0);

	return wifi_rssi;
}


#define SOCKET_DBG "SOCKET:"

static void socket_layer_cb(SOCKET sock, uint8_t evt, void *msg)
{
	printf(SOCKET_DBG "socket_layer_cb " EOL);

	switch(evt)
	{
		case SOCKET_MSG_SENDTO:
		{
			int bytes_sent = *(int16_t *)msg;
			if (bytes_sent > 0)
			{
				/* winc1500_packet_sent(); */
				LOG(SOCKET_DBG "msg sent %db" EOL, bytes_sent);
			}
			else
			{
				LOG(SOCKET_DBG "msg sending failed !!! " EOL);
				/* Reset because of socket re-initialization problem */
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
				recvfrom(sock, socket_recv_buff, MTU, 0);
			} else {
				LOG(SOCKET_DBG "ERROR: bind!" EOL);
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
#if 0

					if (memcmp(&current_sock_desc->remote.sin_addr.s_addr,
					           &raddr[0],
					           sizeof(current_sock_desc->remote.sin_addr.s_addr)) == 0)
#endif
					{
                        LOG(SOCKET_DBG "INFO: received %d [B] from %u.%u.%u.%u:%u <========" EOL,
						datagram->s16BufferSize, raddr[0], raddr[1], raddr[2], raddr[3], _ntohs(rport));
						int16_t len = datagram->s16BufferSize;
						/* ASSERT(len >= 0); */

						recvfrom(sock, socket_recv_buff, MTU, 0);

/*************** PROX: Handle message received from the Proximetry Portal *****************************************/
						if (sock == prox_agent_get_socket(false))
						{
							prox_agent_recv_msg(  (uint8_t *)datagram->pu8Buffer,len);
						}
/******************************************************************************************************************/

						if (datagram->u16RemainingSize)
						{
							LOG(SOCKET_DBG "WARNING: u16RemainingSize %u" EOL,datagram->u16RemainingSize);
						}
					}
#if 0
					else
					{
						/* Received message not from Proximetry Portal */
						SOCKET_DEBUG_LOG(SOCKET_DBG "WARNING: datagram from %u.%u.%u.%u:%u length: %u" EOL,
							raddr[0], raddr[1], raddr[2], raddr[3], _ntohs(rport), datagram->s16BufferSize);
					}
#endif
				}
				else if (datagram->s16BufferSize == SOCK_ERR_TIMEOUT)
				{
					/* Prepare next buffer reception. */
					LOG(SOCKET_DBG "WARNING: recvfrom timeout" EOL);
				}
				else
				{
					LOG(SOCKET_DBG "ERROR: datagram from %u.%u.%u.%u:%u buff [%p] length: [%u]" EOL,
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
 * \brief Obtain light level
 */
int16_t io1_read_illuminance_blk(void)
{
	uint16_t voltage;

	adc_sync_read_channel(&IO1_LIGHT_SEN_ADC_0, 0, (uint8_t*)&voltage, 2);

	// the following code converts analog read result to lux
	// the actual formula is (3.3 - (voltage * 3.3) / 2^12) * 20.0
	// which can be reduced to ((4096 - voltage) * 66) / 4096;

	return (int16_t)((((uint32_t)4096 - voltage) * 66) / 4096);
}

/********* PROX: Callbacks used by the Proximetry Agent to get Statistic values **********************************/

/*
 * @brief Proximetry Agent get io1_temp statistic callback
 */
float prox_callback__get_io1_temp(void)
{
	return temperature_sensor_read(AT30TSE75X);
}

/*
 * @brief Proximetry Agent get sw0 statistic callback
 */
int32_t prox_callback__get_sw0(void)
{
	return (int32_t) gpio_get_pin_level(SW0) ? 0 : 1;
}

/*
 * @brief Proximetry Agent get ilum statistic callback
 */
int32_t prox_callback__get_io1_ilum(void)
{
	return (int32_t)io1_read_illuminance_blk();
}

/*
 * @brief Proximetry Agent get rssi statistic callback
 */
float prox_callback__get_rssi(void)
{
	return  (float)wifi_get_rssi_blk();
}
/******************************************************************************************************************/



/********* PROX: Callbacks used by the Proximetry Agent to set Configuration Parameters  **************************/
/*
 * @brief Proximetry Agent Configuration Parameters change callback
 */
void prox_callback__set_led_freq(int32_t led_freq)
{
	set_led0_blinking_freq(led_freq);
}

/*
 * @brief Proximetry Agent Configuration Parameters change callback
 */
void prox_callback__set_led_onoff(int32_t led_onoff)
{
	if (led_onoff)
		led0_on();
	else
		led0_off();
}
/******************************************************************************************************************/


/**
 * @brief Pushes parameter values to the Proximetry Library
 */
static void prox_push_conf_params(void)
{
	prox_conf_param_push__led_onoff(get_led0_service_state());
	prox_conf_param_push__led_freq(get_led0_freq());
}

/**
 * @brief Updates Alert states in the Proximetry Library
 *
 * This function is called by the Agent thread to update Alert states.
 * Add here code responsible for setting and clearing Alerts.
 */
static void prox_alerts_service(void)
{

#define ALARM_OFF  false
#define ALARM_ON   true
#define TEMPERATURE_ALERT_ON_LEVEL    29.0
#define TEMPERATURE_ALERT_OFF_LEVEL   28.0

    static bool alert1 = ALARM_OFF;
    static bool alert2 = ALARM_OFF;

    float temp = temperature_sensor_read(AT30TSE75X);

    //io1_temp  - temperature alarm with hysteresis
    if( temp > TEMPERATURE_ALERT_ON_LEVEL && alert1 == ALARM_OFF){
        LOG("alert1 on: Temperature > " FLOAT_TO_INT_POINT_INT_FORMAT EOL,FLOAT_TO_INT_POINT_INT_VALUES(TEMPERATURE_ALERT_ON_LEVEL));
        prox_alert_set__io1_temp();
        alert1 = ALARM_ON;
    }else if (temp < TEMPERATURE_ALERT_OFF_LEVEL && alert1 == ALARM_ON ){
        LOG("alert1 off: Temperature < " FLOAT_TO_INT_POINT_INT_FORMAT EOL,FLOAT_TO_INT_POINT_INT_VALUES(TEMPERATURE_ALERT_OFF_LEVEL));
        prox_alert_clear__io1_temp();
        alert1 = ALARM_OFF;
    }

    //sw0
    if (0 == gpio_get_pin_level(SW0) && alert2 == ALARM_OFF){
        LOG("alert2 on: SW0"EOL);
        prox_alert_set__sw0();
        alert2 = ALARM_ON;
    }else if (1 == gpio_get_pin_level(SW0) && alert2 == ALARM_ON){
        LOG("alert2 off: SW0"EOL);
        prox_alert_clear__sw0();
        alert2 = ALARM_OFF;
    }
}

/**
 * \brief main
 */
int main(void)
{
	system_init();

	TIMER_0_init();

	temperature_sensors_init();
	adc_sync_enable_channel(&IO1_LIGHT_SEN_ADC_0, 0);

	printf(STRING_HEADER);

	/* Initialize wifi module */
	tstrWifiInitParam param;
	param.pfAppWifiCb = wifi_cb;
	wifi_init(&param);
/*********** PROX: Push the MAC address to the Proximetry Library **************************************************/
	prox_conf_param_push__mac((char*)get_mac_str());
/******************************************************************************************************************/

	/* Initialize Socket module */
	socketInit();
	registerSocketCallback(socket_layer_cb, NULL);

/********** PROX: Push the Configuration Parameters to the Proximetry Library *************************************/
 	prox_push_conf_params();
/******************************************************************************************************************/

/********** PROX: Proximetry Agent initialization *****************************************************************/
	prox_agent_init();
/******************************************************************************************************************/

	while(1) {

		m2m_wifi_handle_events(NULL);

		if (gbConnectedWifi)
		{
/********** PROX: Proximetry Agent service task *******************************************************************/
				prox_agent_task();
/******************************************************************************************************************/
		}
		else
		{
			/* [Re]connect to wifi */
			if (connect_timer < time_ms())
			{
				connect_timer = time_ms() + CONNECTION_TIMEOUT;
				connect_wifi();
			}
		}

		led0_service();

/********** PROX: Proximetry Agent Alerts service *****************************************************************/
		prox_alerts_service();
/******************************************************************************************************************/

	}

}
