#ifndef MAIN_H
#define MAIN_H

// <<< Use Configuration Wizard in Context Menu >>>
// <s> Network SSID
	// <i> The name (SSID) of WiFi network to connect to
	// <id> def_ssid
#ifndef DEF_SSID
#   define DEF_SSID    "SSID"
#endif

// <s> Network password
	// <i> The password used to connect to WiFi network
	// <id> def_password
#ifndef DEF_PASSWORD
#   define DEF_PASSWORD    "Password"
#endif

// <o> Security type
	// <1=> Wi-Fi network is not secured
	// <2=> WPA/WPA2 personal(PSK)
	// <3=> WEP (40 or 104) OPEN OR SHARED
	// <4=> WPA/WPA2 Enterprise.IEEE802.1x
	// <i> Network security
// <id> def_sec_type
#ifndef DEF_SEC_TYPE
#   define DEF_SEC_TYPE 2
#endif
// <<< end of configuration section >>>

/**
 * \brief Obtain time stamp
 *
 * \return Time stamp in ms.
 */
uint64_t dev_now(void);

/**
 * \brief Callback to get the Wi-Fi status update.
 *
 * \return Wi-Fi IP address as string
 */
const char *get_ip_str(void);

/**
 * \brief Set the blinking frequency for LED0
 *
 * \param[in] f A frequency to set
 *
 * \return The status of setting the frequency
 */
int set_led0_blinking_freq(int32_t f);

/**
 * \brief Turn LED0 on
 *
 * \return The status of operation
 */
int led0_on(void);

/**
 * \brief Turn LED0 off
 *
 * \return The status of operation
 */
int led0_off(void);

/**
 * \brief Obtain the state of LED0 (on/off)
 *
 * \return true if LED is on, false if LED is off
 */
bool get_led0_service_state(void);

/**
 * \brief Obtain the blinking frequency of LED0
 *
 * \return The blinking frequency
 */
int32_t get_led0_freq(void);

/**
 * \brief Blinking with led0_blinking_freq frequency
 */
void led0_service(void);

/**
 * \brief Obtain wifi connection status
 *
 * \return wifi connection status
 */
bool wifi_connected(void);

/**
 * \brief Perform blocking rssi wifi request
 *
 * /return rssi
 */
int8_t wifi_get_rssi_blk(void);
#endif
