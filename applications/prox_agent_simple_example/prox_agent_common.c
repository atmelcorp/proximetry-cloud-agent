/*******************************************************************************
 *
 * AMP Agent example implementation
 *
 * Copyright (c) 2015 Proximetry, Inc.
 * All Rights Reserved
 *
 ******************************************************************************/
/**
 * @file prox_common.c
 * @brief
 * */
#include <stdlib.h>
#include "atmel_start.h"
#include "driver/include/m2m_wifi.h"
#include "prox_agent_common.h"

/**
 * \brief Concatenate 4 IP address digits into one 4-byte variable
 */
static inline uint32_t ip_int(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	union { uint32_t i; uint8_t ip[4]; } ip_int = {.ip = {a, b, c, d}};
	return ip_int.i;
}

/**
 * \brief Read IPv4 address from string
 */
uint32_t read_ip(const char *str)
{
	const char *ip0 = str;
	const char *ip1 = strchr(ip0, '.');
	if (ip1++ == NULL)
		return 0;
	const char *ip2 = strchr(ip1, '.');
	if (ip2++ == NULL)
		return 0;
	const char *ip3 = strchr(ip2, '.');
	if (ip3++ == NULL)
		return 0;
	return ip_int(atoi(ip0), atoi(ip1), atoi(ip2), atoi(ip3));
}

/**
 * \brief Obtain own MAC address as string
 *
 * \return MAC address
 */
const char *get_mac_str(void)
{
	static char mac_str[19];
	uint8_t mac[6];
	if (m2m_wifi_get_mac_address(mac) != M2M_SUCCESS)
	{
		error_stop("Unable to get wifi mac address");
	}
	sprintf(mac_str,"%02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	return mac_str;
}

/**
 * \brief Stop execution on error
 */
void error_stop(char *msg)
{
	printf("%s"EOL,msg);
	printf("Error stop"EOL);

	while(1)
		;
}

/**
 * \brief Obtain light level
 */
int16_t io1_read_illuminance_blk(void)
{
	uint16_t voltage;

	adc_sync_read(&IO1_LIGHT_SEN_ADC_0, (uint8_t*)&voltage, 2);

	// the following code converts analog read result to lux
	// the actual formula is (3.3 - (voltage * 3.3) / 2^12) * 20.0
	// which can be reduced to ((4096 - voltage) * 66) / 4096;

	return (int16_t)((((uint32_t)4096 - voltage) * 66) / 4096);
}

