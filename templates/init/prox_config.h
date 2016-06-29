// ******************************************************************************************
// <h> Proximetry Portal Settings
// ******************************************************************************************

// <s> Activation code
// <i> Code associated with a Proximetry Portal user account
// <id> prox_activation_code
#define PROX_ACTIVATION_CODE "PUT YOUR ACTIVATION CODE HERE"

// </h>


// ******************************************************************************************
// <h> Agent Settings
// ******************************************************************************************

// <s> Device name
// <i> Name associated with device
// <id> prox_device_name
#define PROX_DEVICE_NAME "SAMD21_STARTNAME"

// <o> Device model id <0x0-0xFFFFFFFF>
// <i> The Device Model Id is crucial and must be consistent with a Device Model Id deployed on the Proximetry Portal
// <id> prox_device_model_id
#define PROX_DEVICE_MODEL_ID 0x00000000

// <o> Device model version <0-1000>
// <i> The Device Model Version is crucial and must be consistent with a Device Model Version deployed on the Proximetry Portal
// <id> prox_device_model_version
#define PROX_DEVICE_MODEL_VERSION 0

// </h>


// ******************************************************************************************
// Miscellaneous
// ******************************************************************************************

//the sending synchronous message interval
#define PROX_SYNC_INTERVAL 10

// Proximetry Portal port number
#define PROX_PORT 5050

// Proximetry Portal ip address
#define PROX_SERVER_IP "54.191.88.84"


// ******************************************************************************************
// Logger
// ******************************************************************************************

#ifndef PROX_LOGGING
	/* Uncomment to enable the logging */
	#define PROX_LOGGING
#endif

#ifdef PROX_LOGGING
	#define PROX_LOG_PREFIX "prox: "
	#define PROX_LOG(...)   do{	printf("%s",PROX_LOG_PREFIX); printf(__VA_ARGS__); }while(0)
	#ifndef EOL
		#define EOL   "\n\r"
	#endif
	#define PROX_LOG_LEVEL  PROX_LOG_INFO_E
#else
	#define PROX_LOG_PREFIX NULL
	#define PROX_LOG(...)
	#define PROX_LOG_LEVEL  PROX_LOG_DISABLE_E
#endif


// ******************************************************************************************
// Config Parameters, Alerts and Statistics
// ******************************************************************************************

