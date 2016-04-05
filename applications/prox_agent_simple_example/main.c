#include <string.h>
#include <stdlib.h>

#include "atmel_start.h"
#include "winc1500_main.h"
#include "stdio_start.h"

#include "socket/include/socket.h"
#include "socket_layer.h"

#include "driver/include/m2m_wifi.h"
#include "prox_agent.h"
#include "at30tse75x.h"
#include "main.h"
#include <temperature_sensor_main.h>

#define CONNECTION_TIMEOUT   	10000  						//[ms] - timeout for wifi [re]connection

#include "prox_agent_helper.h"

#define APP_VER "0.1.0"
#define STRING_HEADER EOL EOL "Start Proximetry Agent Simple Example " APP_VER " --" EOL \

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

/**
 * \brief Callback to get the Wi-Fi status update.
 */
const char *get_ip_str(void)
{
	return wifi_ip_str;
}

/**
 * \brief Obtain wifi connection status
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
			prox_init_udp_socket();
			prox_agent_update_network_param();
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
 * \brief function gives time stamp
 */
uint64_t dev_now(void)
{
	return timer_1ms;
}

/**
 * \brief Timer 0 task callback
 */
static void TIMER_0_task1_cb(const struct timer_task *const timer_task)
{
	timer_1ms++;
}

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
		if (next_blink < dev_now()){
			/* positive freq */
			if (led0_blinking_freq > 0)
				next_blink = dev_now() + (1000/led0_blinking_freq)/2;
			/* negative freq */
			else if (led0_blinking_freq < 0)
				next_blink = dev_now() + (-led0_blinking_freq) * 1000/2;

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
 * \brief Perform blocking rssi wifi request
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


/**
 * \brief main
 */
int main(void)
{
	system_init();

	STDIO_REDIRECT_0_init();
    TIMER_0_init();
	adc_sync_enable(&IO1_LIGHT_SEN_ADC_0);
	temperature_sensors_init();

	printf(STRING_HEADER);

	/* Initialize wifi module */
	tstrWifiInitParam param;
	param.pfAppWifiCb = wifi_cb;
	wifi_init(&param);

	/* Initialize Socket module */
	winc1500_socket_layer_init();

	prox_cloud_agent_init();

	while(1) {

		m2m_wifi_handle_events(NULL);

		if (gbConnectedWifi)
		{
				prox_cloud_agent_thread_nb();
		}
		else
		{
			/* [Re]connect to wifi */
			if (connect_timer < dev_now())
			{
				connect_timer = dev_now() + CONNECTION_TIMEOUT;
				connect_wifi();
			}
		}

		led0_service();
	}
}
