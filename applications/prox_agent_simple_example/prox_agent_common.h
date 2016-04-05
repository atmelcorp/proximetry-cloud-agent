/*******************************************************************************
 *
 * AMP Agent example implementation
 *
 * Copyright (c) 2015 Proximetry, Inc.
 * All Rights Reserved
 *
 ******************************************************************************/
/**
 * @file common.h
 * @brief
 * */


#ifndef COMMON_H
#define COMMON_H

#include <utils.h>
#include <stdio.h>
#include <string.h>
#include "prox_agent_amp.h"

#define UNUSED(v)          (void)(v)

#define LOG(...)   do{	printf("%s",PROX_AGENT_LOG_PREFIX); printf(__VA_ARGS__); }while(0)

#define EOL   "\n\r"

/**
 * @brief get number of elements of an array
 */
#ifndef ARRAY_ELEMS
    #define ARRAY_ELEMS(arr) ARRAY_SIZE(arr)
#endif

#ifndef STR
#define STR(x) #x
#endif

#define PRINT_MAC_FORMAT      "%02X:%02X:%02X:%02X:%02X:%02X"
#define PRINT_MAC_FORMAT_2    "%02X-%02X-%02X-%02X-%02X-%02X"
#define PRINT_MAC_FORMAT_3    "%02X%02X%02X%02X%02X%02X"
#define PRINT_MAC_VALUES(mac)  (mac)[0], (mac)[1], (mac)[2], (mac)[3], (mac)[4], (mac)[5]

#define SCANF_MAC_FORMAT      "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx"
#define SCANF_MAC_FORMAT_2    "%02hhx-%02hhx-%02hhx-%02hhx-%02hhx-%02hhx"
#define SCANF_MAC_VALUES(mac)  (mac), (mac) + 1, (mac) + 2, (mac) + 3, (mac) + 4, (mac) + 5

#define PRINT_IP_FORMAT       "%d.%d.%d.%d"
#define PRINT_IP_VALUES(ip)    (ip)[0], (ip)[1], (ip)[2], (ip)[3]

#define SCANF_IP_FORMAT       "%hhd.%hhd.%hhd.%hhd"
#define SCANF_IP_VALUES(ip)    (ip), (ip) + 1, (ip) + 2, (ip) + 3

#define FLOAT_TO_INT_POINT_INT_FORMAT  		"%d.%d"
#define FLOAT_TO_INT_POINT_INT_VALUES(x) 	(int)x,(int)((int)(x*100)%100)

/**
 * \brief Read IPv4 address from string
 *
 * \param[in] str   address as string
 *
 * \return address as a series of 4 bytes in host endianness
 */
uint32_t read_ip(const char *str);

/**
 * \brief Obtain own MAC address as string
 *
 * \return MAC address
 */
const char *get_mac_str(void);

/**
 * \brief Obtain light level
 *
 * \return Current light level
 */
int16_t io1_read_illuminance_blk(void);

/**
 * \brief Stop execution on error
 *
 * Prints error message and stops program execution
 *
 * \param[in] msg Error message to print
 */
void error_stop(char *msg);

#endif /* COMMON_H */
