/*******************************************************************************
 *
 * AMP Agent
 *
 * Copyright (c) 2015 Proximetry Inc. All rights reserved.
 * Authors: Proximetry
 * Copyright (c) 2015 Proximetry, Inc.
 * All Rights Reserved
 *
 ******************************************************************************/
/**
 * @file prox_agent_helper.h
 * @brief AMP Agent
 * */
#ifndef PROX_AGENT_HELPER_H
#define PROX_AGENT_HELPER_H


/*
 * Uncoment it if the helper api has been already implemented
 */
#define PROXIMETRY_AGENT_HELPER

/**
 * These functions set is an interface between agent and the application environment.
 * This functions are used by agent application to get values related to system state and to
 * fill parameters,statistics and alarms associated with them.
 */

/**
 * @brief
 */
uint64_t prox_agent_now(void);
/**
 * @brief Proximetry Agent change parameters indicator
 *
 * This function is invoked by the agent if given parameter has been changed.
 * Here is the place for adding the logic/action when the parameters is changed.
 */
void prox_agent_param_changed(uint32_t id);

/**
 * @brief Proximetry Agent set parameters function
 *
 * This function should contains the code which fill in the agent parameters with their current values.
 * The function is called during agent initialization, and may be considered as
 * parameter initial function. To set the parameters the prox_agent_amp.h api should being used.
 *
 */
int prox_agent_set_params(void);

/**
 * @brief Proximetry Agent update network parameters
 *
 * This function should be called from a dhcp callback when the ip settings are valid.
 * This function is responsible for updates the parameter which send an ip address of device to the cloud server in this project.
 *
 * @ return Status of updating
 */
int prox_agent_update_network_param(void);

/**
 * @brief Proximetry Agent set statistics function
 *
 * This function should contains the code which fill in the agent statistics with their current values.
 * The function is called, by agent library, just before sending every synchronize message (periodically in every synchronize interval).
 * To set the statistics the prox_agent_amp.h api should being used.
 */
void prox_agent_stats_update(void);
/**
 * @brief Proximetry Agent set alarms function
 *
 * This function should contains the code which fill in the agent alarms with their current values.
 * The function is called periodically by agent.
 * To set/clear the alarms the prox_agent_amp.h api should being used.
 */
void prox_agent_alarms_update(void);

/**
 * @brief Proximetry Agent get device name
 *
 * This function is utilizes by agent to get the device name which will be reported to cloud server.
 * Proximetry Device Name is a max 32 chars width string;
 */
const char *prox_get_device_name(void);

/**
 * @brief Proximetry Agent get device name
 *
 * This function is utilizes by agent to get the device id.
 * The device id should be unique for each device. The best candidate for the device_id is a mcu serial number.
 * Proximetry Device_ID is a max 100 chars width string.
 */
const char *prox_get_device_id(void);

#endif
