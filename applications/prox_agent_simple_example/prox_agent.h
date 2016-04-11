/*******************************************************************************
 *
 * Proximetry AMP Agent Application
 *
 * Copyright (c) 2016 Proximetry Inc. All rights reserved.
 * Authors: Proximetry
 *
 ******************************************************************************/

/**
 * @file prox_agent.h
 * @brief Proximetry CLoud Agent
 * */

#ifndef PROX_AGENT_H
#define PROX_AGENT_H

#include <atmel_start.h>
#include "main.h"
#include "at30tse75x.h"

/**
 * @brief function return timestamp [ms]
 */
uint64_t prox_agent_time_ms(void);

/**
 *
 * */
int prox_agent_send_msg(uint8_t *data, uint16_t size);

/**
 * @brief Proximetry Cloud Agent initialization funtion
 */
int prox_init_udp_socket(void);

/**
 * @brief Proximetry Agent get device name
 *
 * This function is utilizes by agent to get the device id.
 * The device id should be unique for each device. The best candidate for the device_id is a mcu serial number.
 * Proximetry Device_ID is a max 100 chars width string.
 */
const char *prox_get_device_id(void);

/**
 * @brief Proximetry Agent get device name
 *
 * This function is utilizes by agent to get the device name which will be reported to cloud server.
 * Proximetry Device Name is a max 32 chars width string;
 */
const char *prox_get_device_name(void);


/**
 * @brief Proximetry Agent get an activation code
 *
 */
const char *prox_get_activation_code(void);

/**
 * @brief Proximetry Agent get a synchronization interval value
 *
 */
const unsigned int prox_get_sync_interval(void);

#endif /* PROX_AGENT_H */
