/*******************************************************************************
*
* Proximetry AMP Agent Application
*
* Copyright (C) 2016 Proximetry Inc. All rights reserved.
* Authors: Proximetry (www.proximetry.io)
*
* THIS SOFTWARE IS PROVIDED BY PROXIMETRY "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL PROXIMETRY BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/

/**
 * @file prox_core.h
 * @brief Proximetry Cloud Agent - System Interface
 * */

#ifndef prox_agent_h
#define prox_agent_h

#include <stdint.h>

/**
 * @brief Proximetry Agent Application initialization
 *
 * Initializes the Proximetry Agent Library,
 * initializes the Proximetry Agent Application parameters.
 */
int prox_agent_init(void);

/**
 * @brief Proximetry Agent application task (non-blocking)
 *
 * Updates the alerts states in the Proximetry Agent Library,
 * performs the agent functionality,
 *
 * It should be periodically invoked by the main task, at least once per sync message interval.
 */
void prox_agent_task(void);

/**
 * @brief Proximetry Agent get socket funtion
 *
 * Function returns module's socket descriptor.
 * If socket is not created the function creates it.
 *
 * @param[in]:	init	1 - re-initialize the socket
 */
int prox_get_socket(bool init);

/**
 * @brief Proximetry Agent receive message
 *
 * This routine must by invoked by the system when a massage, for the Proximetry Agent, arrived.
 */
void prox_recv_msg(uint8_t *buffer, size_t len);

/**
 * @brief Proximetry Agent get time
 *
 * This routine is invokes by Proximetry Agent Library.
 * It must retrieves timestamp from the system.
 *
 */
extern uint64_t prox_callback__get_time_ms(void);

/*
 * @brief 
 */
extern const char* prox_callback__get_device_id(void);
extern const char* prox_callback__get_device_name(void);
extern const char* prox_callback__get_activation_code(void);
void prox_set_sync_interval(uint32_t time_interval);
#endif /* prox_agent_h */
