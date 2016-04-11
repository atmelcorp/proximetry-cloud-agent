/*******************************************************************************
 *
 * Proximetry AMP Agent Application
 *
 * Copyright (c) 2016 Proximetry Inc. All rights reserved.
 * Authors: Proximetry
 *
 ******************************************************************************/
/**
 * @file prox_agent_amp.h
 * @brief Proximety Cloud Agent AMP
 * */
#ifndef PROX_AGENT_AMP_H
#define PROX_AGENT_AMP_H

#include "amp_agent.h"

/* Parameters IDs */
#define PROX_AGENT_PARAM_ID_STR_MAC 1001
#define PROX_AGENT_PARAM_ID_STR_IP 1002
#define PROX_AGENT_PARAM_ID_INT32_LED_ONOFF 1003
#define PROX_AGENT_PARAM_ID_INT32_LED_FREQ 1004

/* Statistics IDs */
#define PROX_AGENT_STAT_ID_IO1_ILUM 101
#define PROX_AGENT_STAT_ID_SW0 102
#define PROX_AGENT_STAT_ID_IO1_TEMP 103
#define PROX_AGENT_STAT_ID_WINC_RSSI 104

/* Alarms IDs */
#define PROX_AGENT_ALARM_ID_IO1_TEMP 11
#define PROX_AGENT_ALARM_ID_SW0 12

#define PROX_AGENT_LOG_PREFIX "prox_agent: "
#define PARAM_STR_LEN 100

int prox_agent_init(void);

/**
 * @brief Proximetry Cloud Agent non-blocking service function
 *
 * It should be periodically invoked by the main task, at least once per set sync message interval.
 */
void prox_agent_thread_nb(void);

#endif
