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
 * @brief Proximetry Library - Application Interface
 */

#ifndef __PROXAPI_H__
#define __PROXAPI_H__

#include <stdbool.h>
#include <stdint.h>

// Proximetry Library functions
/**
 * @brief  Proximetry Library version
 * @return Proximetry Library version (string)
 */
const char* prox_get_library_version(void);
/**
 * @brief Function returns state of the connection to the Proximetry Portal.
 * @return true if authentication to the Proximetry Portal has been finished successfully.
 */
bool prox_is_connected(void);

/**
 * @brief Notify the Proximetry Portal that configuration has been changed
 *
 * This functions must be called when the parameters have been changed locally to synchronize the changes with the Proximetry Portal.
 */
void prox_conf_param_set(void);

/**
 * @brief Write configuration parameter
 *
 * The functions below are used to write configuration parameter values to the Proximetry Library.
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
 * The functions below are used to read a configuration parameter value from the Proximetry Library.
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
 * The functions below are used to write statistic values to the Proximetry Library.
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
 * @brief Handle alerts
 *
 * The functions below are used to set Alert states in the Proximetry Library.
 */
void prox_set_alert(uint32_t id);
void prox_clear_alert(uint32_t id);

#endif /* __PROXAPI_H__ */
