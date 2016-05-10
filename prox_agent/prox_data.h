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
 * @file prox_data.h
 * @brief Proximety Cloud Agent AMP
 * */

#ifndef PROX_DATA_H
#define PROX_DATA_H

#define PROX_SDK_VER_MAJOR     2
#define PROX_SDK_VER_MINOR     0
#define PROX_SDK_VER_REVISION  1
#define PROX_SDK_VER_INSTANCE  1     // if 0 then it is a pure copy of sdk without local changes.
                                     // Increment it when the sdk code is modified due to project specific requirements.

#define PROX_SDK_VER       TOSTR(PROX_SDK_VER_MAJOR)"."TOSTR(PROX_SDK_VER_MINOR)"."TOSTR(PROX_SDK_VER_REVISION)"."TOSTR(PROX_SDK_VER_INSTANCE)

/*
 * @brief Statically allocated variables for library
 */
extern prox_vars_t prox_vars;

/**
 * @brief Parameter changed callback function
 */
void prox_conf_param_changed(uint32_t param_id);

/**
 * @brief Update statistic values in the Proximetry Agent Library
 */
void prox_stats_update(void);

/**
 * @brief Set parameter values in the Proximetry Agent Library
 */
int prox_conf_params_update(void);

/**
 * @brief Update alert states in the Proximetry Agent Library
 */
void prox_alerts_update(void);

/*
 * @brief Dump configuration parameters values from library
 */
void prox_dump_conf_params(void);

/**
 * @brief Proximetry Agent get device id
 */
const char *prox_get_device_id(void);

/**
 * @brief Proximetry Agent get a device name
 */
const char *prox_get_device_name(void);

/**
 * @brief Proximetry Agent get an activation code
 */
const char *prox_get_activation_code(void);

/**
 * @brief Proximetry Agent get a synchronization interval value
 */
unsigned int prox_get_sync_interval(void);

/**
 * @brief Proximetry Agent set a synchronization interval value
 */
void prox_set_sync_interval(uint32_t time_interval);

#endif

