/*******************************************************************************
 *
 * Copyright (c) 2014 Proximetry Inc. All rights reserved.
 * Authors: Proximetry
 * All Rights Reserved
 *
 ******************************************************************************/
/**
 * @file prox_agent.h
 * @brief Proximetry CLoud Agent
 * */
#ifndef PROX_AGENT_H
#define PROX_AGENT_H


#define EXAMPLE_AGENT_VERSION "0.1.0"


/**
 * @brief Proximetry Cloud Agent initialization funtion
 */
int  prox_init_udp_socket(void);

/**
 * @brief Proximetry Cloud Agent initialization funtion
 */
void prox_cloud_agent_init(void);

/**
 * @brief Proximetry Cloud Agent non-blocking service function
 *
 * It should be periodically invoked by the main task, at least once per set sync message interval.
 */
void prox_cloud_agent_thread_nb(void);

#endif /* PROX_AGENT_H */
