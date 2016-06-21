/*******************************************************************************
*
* Proximetry Agent Library
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
 * @file prox.h
 *
 * @brief Proximetry Library - Programming Interface
 */

#ifndef __PROX_H__
#define __PROX_H__


#include <stdbool.h>
#include <stdint.h>
#include "prox_api.h"

/*----------------------------------------------------------------------------
 * Version 2.3.0
 *----------------------------------------------------------------------------*/

// Proximetry Library static configuration. Fixed for a library instance.
// This mean that for compiled library package these are here for information purpose only.
// #define PROX_CFG_MTU 1500            /* @brief MTU of underlying network protocol */

typedef enum
{
	PROX_LOG_DISABLE_E = 0,
	PROX_LOG_CRITICAL_E,
	PROX_LOG_ERROR_E,
	PROX_LOG_WARNING_E,
	PROX_LOG_INFO_E,
	PROX_LOG_DEBUG_E
}prox_log_level_t;

/*----------------------------------------------------------------------------*/
// Proximetry Library data types
/**
 * @brief Enum defining supported data types
 */
typedef enum
{
	PROX_TYPE_BOOL   = 0,
	PROX_TYPE_INT8   = 1,
	PROX_TYPE_INT16  = 2,
	PROX_TYPE_INT32  = 3,
	PROX_TYPE_UINT8  = 4,
	PROX_TYPE_UINT16 = 5,
	PROX_TYPE_UINT32 = 6,
	PROX_TYPE_FLOAT  = 7,
	PROX_TYPE_STRING = 9

} prox_type_t;

/**
 * @brief Union for storing different value types
 */
typedef union
{
	bool      b;    /**< @brief boolean value */
	int8_t    i8;   /**< @brief signed 8bit integer value */
	int16_t   i16;  /**< @brief signed 16bit integer value */
	int32_t   i32;  /**< @brief signed 32bit integer value */
	uint8_t   ui8;  /**< @brief unsigned 8bit integer value */
	uint16_t  ui16; /**< @brief unsigned 16bit integer value */
	uint32_t  ui32; /**< @brief unsigned 32bit integer value */
	float     f;    /**< @brief float value */
	char     *s;    /**< @brief string value */
} prox_value_t;

/**
 * @brief Abstract type for storing different data objects
 */
typedef struct
{
	uint32_t    id;     /**< @brief data id */
	prox_type_t  type;   /**< @brief data type */
	prox_value_t value;  /**< @brief data value */

} prox_data_t;

/**
 * @brief Type for Statistic data
 */
typedef prox_data_t prox_stat_t;

/**
 * @brief Type for Configuration Parameter data
 */
typedef prox_data_t prox_conf_param_t;

/**
 * @brief Enum defining Alert states
 */
typedef enum
{
	PROX_ALERT_STATE_CLEARED = 0,
	PROX_ALERT_STATE_SET     = 1

} prox_alert_state_t;

/**
 * @brief Type for Alert data
 */
typedef struct
{
	uint8_t             id;         /**< @brief alert id */
	prox_alert_state_t  state;      /**< @brief alert state */
	uint8_t             chg_count;  /**< @brief alert state change counter */

} prox_alert_t;

/**
 * @brief A structure containig basic application configuration data required to initialize the Proximetry Library
 */
typedef struct
{
	const char *device_id;         /**< @brief device ID */
	const char *device_name;       /**< @brief device name */
	uint32_t    model_id;          /**< @brief device model ID */
	uint16_t    model_ver;         /**< @brief device model version ID */
	int         sync_msg_interval; /**< @brief periodic message reporting interval in [s] */
	const char *activation_code;   /**< @brief activation code */

} prox_settings_t;

/**
 * @brief A structure containig callback definitions required to initialize the Proximetry Library
 */
typedef struct
{
	uint64_t (*get_time_ms) (void);
	void 	 (*conf_param_changed) (uint32_t conf_param_id);
	void 	 (*get_data) (void);
	int 	 (*send_msg) (uint8_t *data, uint16_t size);
} prox_callbacks_t;

/**
 * @brief A structure containig application data definitions required to initialize the Proximetry Library  
 */
typedef struct
{
	prox_conf_param_t    *params;
	uint32_t        params_num;

	prox_stat_t     *stats;
	uint32_t        stats_num;

	prox_alert_t    *alerts;
	uint32_t        alerts_num;

} prox_vars_t;


/*----------------------------------------------------------------------------*/
// Proximetry Library macros
/**
 * @brief Define bool parameter
 * Use this macro to initialize bool params in parameters memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_BOOL_CONF_PARAM_DEF(_id) { (_id), PROX_TYPE_BOOL, {.b = false} }

/**
 * @brief Define 8bit signed integer parameter
 * Use this macro to initialize int8 params in parameters memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_INT8_CONF_PARAM_DEF(_id) { (_id), PROX_TYPE_INT8, {.i8 = 0} }

/**
 * @brief Define 16bit signed integer parameter
 * Use this macro to initialize int16 params in parameters memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_INT16_CONF_PARAM_DEF(_id) { (_id), PROX_TYPE_INT16, {.i16 = 0} }

/**
 * @brief Define 32bit signed integer parameter
 * Use this macro to initialize int32 params in parameters memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_INT32_CONF_PARAM_DEF(_id) { (_id), PROX_TYPE_INT32, {.i32 = 0} }

/**
 * @brief Define 8bit unsigned integer parameter
 * Use this macro to initialize uint8 params in parameters memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_UINT8_CONF_PARAM_DEF(_id) { (_id), PROX_TYPE_UINT8, {.ui8 = 0} }

/**
 * @brief Define 16bit unsigned integer parameter
 * Use this macro to initialize uint16 params in parameters memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_UINT16_CONF_PARAM_DEF(_id) { (_id), PROX_TYPE_UINT16, {.ui16 = 0} }

/**
 * @brief Define 32bit unsigned integer parameter
 * Use this macro to initialize uint32 params in parameters memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_UINT32_CONF_PARAM_DEF(_id) { (_id), PROX_TYPE_UINT32, {.ui32 = 0} }

/**
 * @brief Define float parameter
 * Use this macro to initialize float params in parameters memory buffer
 * @param[in] _id     param ID, integer literal
 * @param[in] _buffer buffer for string params
 */
#define PROX_FLOAT_CONF_PARAM_DEF(_id) { (_id), PROX_TYPE_FLOAT, {.f = 0.0} }

/**
 * @brief Define string parameter
 * Use this macro to initialize string params in parameters memory buffer
 * @param[in] _id     param ID, integer literal
 * @param[in] _buffer buffer for string characters
 */
#define PROX_STRING_CONF_PARAM_DEF(_id, _buffer) { (_id), PROX_TYPE_STRING, {.s = (_buffer)} }

/**
 * @brief Define bool statistic
 * Use this macro to initialize bool stats in statistics memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_BOOL_STAT_DEF(_id) { (_id), PROX_TYPE_BOOL, {.b = false} }

/**
 * @brief Define 8bit signed integer statistic
 * Use this macro to initialize int8 stats in statistics memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_INT8_STAT_DEF(_id) { (_id), PROX_TYPE_INT8, {.i8 = 0} }

/**
 * @brief Define 16bit signed integer statistic
 * Use this macro to initialize int16 stats in statistics memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_INT16_STAT_DEF(_id) { (_id), PROX_TYPE_INT16, {.i16 = 0} }

/**
 * @brief Define 32bit signed integer statistic
 * Use this macro to initialize int32 stats in statistics memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_INT32_STAT_DEF(_id) { (_id), PROX_TYPE_INT32, {.i32 = 0} }

/**
 * @brief Define 8bit unsigned integer statistic
 * Use this macro to initialize uint8 stats in statistics memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_UINT8_STAT_DEF(_id) { (_id), PROX_TYPE_UINT8, {.ui8 = 0} }

/**
 * @brief Define 16bit unsigned integer statistic
 * Use this macro to initialize uint16 stats in statistics memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_UINT16_STAT_DEF(_id) { (_id), PROX_TYPE_UINT16, {.ui16 = 0} }

/**
 * @brief Define 32bit unsigned integer statistic
 * Use this macro to initialize uint32 stats in statistics memory buffer
 * @param[in] _id     param ID, integer literal
 */
#define PROX_UINT32_STAT_DEF(_id) { (_id), PROX_TYPE_UINT32, {.ui32 = 0} }

/**
 * @brief Define float statistic
 * Use this macro to initialize float stats in statistics memory buffer
 * @param[in] _id     stat ID, integer literal
 */
#define PROX_FLOAT_STAT_DEF(_id) { (_id), PROX_TYPE_FLOAT, {.f = 0} }

/**
 * @brief Define alert
 * @param[in]  _id    alert ID, integer literal
 */
#define PROX_ALERT_DEF(_id)    { _id, PROX_ALERT_STATE_CLEARED, 0 }


/*----------------------------------------------------------------------------*/
// Proximetry Library functions

/**
 * @brief Set a logging level and a prefix 
 */
int prox_set_log(prox_log_level_t level, const char *prefix);

/**
 * @brief Initialize the Proximetry Library
 */
int prox_init(prox_settings_t *settings, prox_vars_t *vars, prox_callbacks_t *device_api);

/**
 * @brief Set a Proximetry Agent sync interval
 */
int prox_set_sync_interval(int sync_interval);

/**
 * @brief Process a Proximetry protocol message
 */
void prox_process_msg(uint8_t *msg, uint16_t size);

/**
 * @brief Performs synchronous Agent task
 */
void prox_sync_task(void);


#endif /* __PROX_H__ */
