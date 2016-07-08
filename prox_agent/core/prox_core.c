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
 * @file prox_core.c
 * @brief Proximetry Agent - Platform specific code
 * */

#include <helpers/prox_helpers.h>
#include <prox_config.h>
#include <lib/prox.h>
#include <prox_data.h>
#include "prox_core.h"
#include <prox_agent.h>

#include <atmel_start.h>
#include <driver/include/m2m_wifi.h>
#include <socket/include/socket.h>

static int prox_sock = -1;
static struct sockaddr_in remote;
static bool prox_agent_initialized = false;

/**
 * @brief Proximetry Agent configuration
 * */
static prox_settings_t prox_settings;
static prox_callbacks_t prox_callbacks;

/**
 * @brief Proximetry Agent get device id
 *
 * This function is used by the Proximetry Agent to get a Device ID.
 * Device ID should be unique for each device.
 * Proximetry Device ID can have up to 100 chars.
 */
#pragma weak prox_callback__get_device_id
const char *prox_callback__get_device_id(void)
{
    static char device_serial_no[33];

#define MCU_SERIAL_NR_ADD_1    (uint32_t*)0x0080A00C
#define MCU_SERIAL_NR_ADD_2    (uint32_t*)0x0080A040
#define MCU_SERIAL_NR_ADD_3    (uint32_t*)0x0080A044
#define MCU_SERIAL_NR_ADD_4    (uint32_t*)0x0080A048

	sprintf(device_serial_no, "%08x%08x%08x%08x",
		(unsigned)(*MCU_SERIAL_NR_ADD_1),(unsigned)(*MCU_SERIAL_NR_ADD_2),
		(unsigned)(*MCU_SERIAL_NR_ADD_3),(unsigned)(*MCU_SERIAL_NR_ADD_4));
    device_serial_no[32] = '\0';

    return device_serial_no;
}

/**
 * @brief Proximetry Agent get a device name
 *
 * This function is used by the Proximetry Agent to get a Device name
 * Proximetry Device Name can have up to 32 chars
 *
 */
#pragma weak prox_callback__get_device_name
const char *prox_callback__get_device_name(void)
{
    static char device_name[100];
    uint8_t mac[6];
    if (m2m_wifi_get_mac_address(mac) != M2M_SUCCESS)
    {
        /* error_stop("Unable to get wifi mac address"); */
    }
    sprintf(device_name,"%s_%02X%02X%02X",PROX_DEVICE_NAME,mac[3],mac[4],mac[5]);
    return device_name;
}

/**
 * @brief Proximetry Agent get an activation code
 *
 */
#pragma weak prox_callback__get_activation_code
const char *prox_callback__get_activation_code(void)
{
    return PROX_ACTIVATION_CODE;
}

/**
 * @brief Proximetry Agent get a synchronization interval value
 * @return Periodic message reporting interval value in seconds
 */
static unsigned int get_sync_interval(void)
{
    return PROX_SYNC_INTERVAL;
}

/**
 * @brief Proximetry Agent set a synchronization interval value
 *
 * This function is used to change the periodic reporting interval in runtime.
 *
 */
void prox_agent_set_sync_interval(uint32_t time_interval)
{
	prox_set_sync_interval(time_interval);
}

/**
 * @brief Proximetry Agent get time
 *
 * This function is called by the Proximetry Library.
 * It must return a timestamp in miliseconds
 *
 */
#pragma weak prox_callback__get_time_ms
uint64_t prox_callback__get_time_ms(void)
{
	printf("ERROR: %s default handler. It muse be redefined int the project"EOL,__FUNCTION__);
	printf("HALT"EOL);
	while(1)
	  ;
}


/**
 * @brief Proximetry Agent print library configuration parameters
 */
static void prox_dump_agent_config(void)
{
    PROX_LOG("--- Agent config ---" EOL);
    PROX_LOG("   Device id:         %s" EOL, prox_settings.device_id);
    PROX_LOG("   Device name:       %s" EOL, prox_settings.device_name);
    PROX_LOG("   Model id:          0x%X" EOL, (int)prox_settings.model_id);
    PROX_LOG("   Model ver:         0x%X" EOL, (int)prox_settings.model_ver);
    PROX_LOG("   Sync.msg.interval  %d" EOL, prox_settings.sync_msg_interval);
    PROX_LOG("   Activation code:   %s" EOL, prox_settings.activation_code);
    PROX_LOG("   Cloud Server ip:   %s" EOL, PROX_SERVER_IP);
    PROX_LOG("   Cloud Server port: %u" EOL, PROX_PORT);
}

/**
 * @brief Proximery Agent send message
 *
 * This routine is called by Proximetry Library to send messages
 */
static int prox_send_msg(uint8_t *data, uint16_t size)
{
    /* PROX_LOG("%s: sin_port:%d"EOL,__FUNCTION__,_htons(remote.sin_port)); */
    return  sendto(prox_sock, (void *)data, size, 0, (struct sockaddr*)&remote, sizeof(struct sockaddr_in));
}

/**
 * @brief Proximery Agent receive message
 *
 * This routine must be invoked by the system when a massage from the Proximery Portal is received
 */
void prox_agent_recv_msg(uint8_t *buffer, size_t len)
{
    prox_process_msg((uint8_t*)buffer,len);
}

/**
 * @brief Proximetry Agent socket initialization
 *
 */
static int prox_init_socket(void)
{
    /* allocate network resources */
    struct sockaddr_in local;

    PROX_LOG("Init agent socket"EOL);

    if(prox_sock != -1)
    {
        close(prox_sock);
        prox_sock = -1;
    }

    if ((prox_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("Agent socke error"EOL);
        return -1;
    }

#define PROX_AGENT_LOCAL_PORT  25000
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = 0;
    local.sin_port =  _htons(PROX_AGENT_LOCAL_PORT);
#undef PROX_AGENT_LOCAL_PORT

    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = prox_str2bin_ip(PROX_SERVER_IP);
    remote.sin_port =  _htons(PROX_PORT);

    if (bind(prox_sock, (struct sockaddr *)&local, sizeof(struct sockaddr_in)) != SOCK_ERR_NO_ERROR)
    {
        PROX_LOG("error: failed to bind socket!"EOL);
        close(prox_sock);
        prox_sock = -1;
        return -1;
    }

    return prox_sock;
}

/**
 * @brief Proximety Agent get socket funtion
 *
 * Function returns module's socket descriptor.
 * If socket is not created the function creates it.
 *
 * @param[in]:	init	1 - re-initialize the socket
 */
int prox_agent_get_socket(bool init)
{
    if (prox_sock == -1 || init)
        prox_init_socket();

    return prox_sock;
}

/**
 * @brief Initalize Proximetry Library
 */
static int prox_agent_lib_init(void)
{
    /* fill the library initialization structure */
    prox_settings.model_id            = PROX_DEVICE_MODEL;
    prox_settings.model_ver           = PROX_MODEL_VER;
    prox_settings.sync_msg_interval   = get_sync_interval();

    prox_settings.activation_code   = prox_callback__get_activation_code();
    prox_settings.device_id         = prox_callback__get_device_id();
    prox_settings.device_name       = prox_callback__get_device_name();

    prox_callbacks.conf_param_changed = prox_conf_param_changed;
    prox_callbacks.send_msg           = prox_send_msg;
    prox_callbacks.get_time_ms        = prox_callback__get_time_ms;
    prox_callbacks.get_data 		  = prox_stats_update;

    /* initialize Proximetry Library*/
    prox_init(&prox_settings, &prox_vars, &prox_callbacks);

	return 0;
}

/**
 * @brief Check wheter module is initialized.
 *
 * @param init - 0:return only initialization status 1:if not initialized do initialization
 */
bool prox_is_initialized(void)
{
	return prox_agent_initialized;
}

/**
 * @brief Check if agent is initialized and init Agent Library if not.
 *
 * This function is called by the Proximetry Agent API setters to make sure that the Proximetry Library is properly initialized
 */
void prox_check_and_lib_init(void)
{
	if(!prox_is_initialized())
	{
		prox_set_log(PROX_LOG_DISABLE_E, NULL);
		prox_agent_lib_init();
	}
}

/**
 * @brief Proximetry Agent initialization
 *
 * Initializes the Proximetry Library,
 * initializes the Proximetry Agent Application parameters.
 */
int prox_agent_init(void)
{
    PROX_LOG("agent start:" EOL);
    PROX_LOG("COMPONENT  version: %s" EOL,PROX_COMPONENT_VER);
    PROX_LOG("AGENT      version: %s" EOL,prox_get_agent_version());
    PROX_LOG("LIB        version: %s" EOL,prox_get_library_version());

    PROX_LOG("Cloud ip: %s" EOL,PROX_SERVER_IP);

	// done from thw winc driver level
	//prox_init_socket();

    prox_set_log(PROX_LOG_DEBUG_E, PROX_LOG_PREFIX);
    /* prox_set_log(PROX_LOG_LEVEL, PROX_LOG_PREFIX); */

    /* initialize Proximetry Library*/
    prox_agent_lib_init();

    /* initialize the Proximetry Agent Application parameters*/
    prox_dump_agent_config();

    prox_dump_conf_params();

    prox_agent_initialized = true;

    return 0;
}


/**
 * @brief Proximetry Agent application task (non-blocking)
 *
 * performs the Proximetry Agent functionality,
 *
 * This function must be periodically invoked by the application. It should be called at least once per periodic reporting interval.
 */
void prox_agent_task(void)
{
    if (!prox_agent_initialized)
    {
        PROX_LOG("warning: agent is uninitialized"EOL);
        return;
    }

    prox_sync_task();
}

