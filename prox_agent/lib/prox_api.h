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
 * @file prox_api.h
 * @brief Proximetry Cloud Agent Library - Application Interface
 */

#ifndef __PROXAPI_H__
#define __PROXAPI_H__

#include <stdbool.h>
#include <stdint.h>

// Proximetry Agent Library functions
/**
 * @brief  Proximetry Library version
 * @return Proximetry Library version (string)
 */
const char* prox_get_library_version(void);
/**
 * @brief Check if Proximety Agent is connected to server
 * @return true if connected, false if not connected or uninitialised
 */
bool prox_is_connected(void);

/**
 * @brief Notify cloud server about configuration parameter update by device
 *
 * This functions must be called when the parameters have been change locally,
 * and changes have to by synchronized with a cloud server.
 */
void prox_conf_param_set(void);

/**
 * @brief Write configuration parameter
 *
 * The function have to be used to write a configuration parameter value to the agent library.
 */
int prox_write_conf_param_bool(uint32_t id, bool value);
int prox_write_conf_param_i8(uint32_t id, int8_t value);
int prox_write_conf_param_i16(uint32_t id, int16_t value);
int prox_write_conf_param_i32(uint32_t id, int32_t value);
int prox_write_conf_param_u8(uint32_t id, uint8_t value);
int prox_write_conf_param_u16(uint32_t id, uint16_t value);
int prox_write_conf_param_u32(uint32_t id, uint32_t value);
int prox_write_conf_param_float(uint32_t id, float value);
int prox_write_conf_param_string(uint32_t id, const char *value);

/**
 * @brief Read configuration parameter
 *
 * The function have to be used to read a configuration parameter value from the agent library.
 */
int prox_read_conf_param_bool(uint32_t id, bool *value);
int prox_read_conf_param_i8(uint32_t id, int8_t *value);
int prox_read_conf_param_i16(uint32_t id, int16_t *value);
int prox_read_conf_param_i32(uint32_t id, int32_t *value);
int prox_read_conf_param_u8(uint32_t id, uint8_t *value);
int prox_read_conf_param_u16(uint32_t id, uint16_t *value);
int prox_read_conf_param_u32(uint32_t id, uint32_t *value);
int prox_read_conf_param_float(uint32_t id, float *value);
int prox_read_conf_param_string(uint32_t id, char *value);

/**
 * @brief Write statistic 
 *
 * The function have to be used to write a statistic value to the agent library.
 */
int prox_write_stat_bool(uint32_t id, bool value);
int prox_write_stat_i8(uint32_t id, int8_t value);
int prox_write_stat_i16(uint32_t id, int16_t value);
int prox_write_stat_i32(uint32_t id, int32_t value);
int prox_write_stat_u8(uint32_t id, uint8_t value);
int prox_write_stat_u16(uint32_t id, uint16_t value);
int prox_write_stat_u32(uint32_t id, uint32_t value);
int prox_write_stat_float(uint32_t id, float value);


/**
 * @brief handle alerts
 *
 * The function have to be used to handle alerts in the agent library.
 */
void prox_set_alert(uint32_t id);
void prox_clear_alert(uint32_t id);

#endif /* __PROXAPI_H__ */
