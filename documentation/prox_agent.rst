====================================
!!! THE DOCUMENTAION IS OUTDATED !!!
====================================


================================
Proximetry Cloud Agent Component
================================

The Proximetry Cloud Agent Component allows connecting a device to the Proximetry Cloud Portal.
The component provides the following functionality:

1. Automatic registration with the Cloud Portal
2. An option to monitor device state, attached sensors and alerts 
3. Remote control from the Cloud Portal Web Interface 
  
**Cloud's Data Types**

Cloud Portal differentiates three types of information:

* Statistics,
* Alerts,
* Configuration parameters.

Statistic parameters are data periodically reported by the application to the Cloud Portal. An example statistic can be a temperature or humidity value reported by attached sensors.  
Alerts are used to monitor device health and critical parameters. Once a predefined alert threshold is crossed the Agent reports it to the Cloud Portal. Each alert can either be in the CLEAR or in the SET state. An example alert can be Low Fuel notification in a car. 
Configuration parameters are user-defined settings that controls various aspects of application behavior.  An example configuration parameter can be “Light Control” allowing to turn on or off a light bulb. 
Supported types of particular information are defined below. 
Statistics:  *bool, int8, int16, int32, uint8, uin16, uin32, float, string*

Alerts:  binary values, which can be *set* or *clear*.

Configuration parameters: *bool, int8, int16, int32, uint8, uin16, uin32, float, string*.

The information type determines how the information is presented in the Cloud Portal Web UI. Statistics are displayed as time series on charts, alerts are displayed as notifications to the operator and configuration parameters are displayed on an editable form.  

**Communication**

Proximetry Cloud Agent uses UDP transport layer for two-way communication with the Cloud Portal. The default port used to communicate with the Cloud Portal is UDP 5050. 
Note that some firewalls may block UDP communication. Please contact with your IT department in case you experience any connectivity issues with the Cloud Portal.

**Proximetry Cloud Agent Component Design**

The Proximetry Cloud Agent Component available in the Atmel Start has a predefined set of supported statistics, alerts and configuration parameters.  An application which utilizes the Proximetry Cloud Agent Component is allowed to report up to 4 parameters, 4 statistics and 2 alerts.
The Proximetry Cloud Agent Component provides an API for handling predefined parameters, statistics and alerts. It also provides an interface necessary to handle the Agent module in the project. The Proximetry Cloud Agent Component API is defined in the `prox_agent_amp.h` header file. 
Please see the Proximetry Cloud Agent Example available in the Atmel Start for more information how to integrate the Proximetry Cloud Agent Component with your application.

**Proximetry Cloud Agent Component Integration**

To integrate the Proximetry Cloud Agent Component into a new project the following API functions must be invoked:

* `int prox_agent_amp_init(prox_agent_config_t *config, prox_agent_comm_cb_t *comm_cb);`
* `void amp_agent_sync_task(void);`
* `void amp_agent_handle_msg(uint8_t *msg, uint16_t size);`

and the callbacks defined in the `prox_agent_comm_cb_t` structure must be implemented.
To initialize the Proximetry Cloud Agent Component the `config` and the `comm_cb` structures must be filled out properly. Please read the structure description in the `prox_agent_amp.h` header file for more informaton. 
Look for Proximetry Cloud Agent Example available in the Atmel Start to see an example application using the Proximetry Cloud Agent Component.

**Proximetry Cloud Agent Component Configuration**

* Activation code:
    Device activation code is a 74-char length alphanumeric string assigned to your Cloud Portal account. The device activation code can be found under “Provisioning->Device Activation Codes” in the Cloud Portal main menu.
* Device name
    An initial device name string that will be reported to the Cloud Portal for device identification. This string can be used in the search field to quickly find a device in the Cloud Portal. 
    Note that the device name can be modified by the Operator from the Cloud Portal Web UI be the name. 

Features
--------

   * up to 4 parametrs
   * up to 4 statistics
   * up to 2 alerts


   +-------------+----------------+
   | parameter   | parameter type |
   |             |   (size)       |
   +-------------+----------------+
   | param 1     | string (100)   |
   +-------------+----------------+
   | param 2     | string (100)   |
   +-------------+----------------+
   | param 3     |    int32       |
   +-------------+----------------+
   | param 3     |    int32       |
   +-------------+----------------+

   +-------------+----------------+
   | statistics  | statistic type |
   |             |                |
   +-------------+----------------+
   | statistic 1 |    int32       |
   +-------------+----------------+
   | statistic 2 |    int32       |
   +-------------+----------------+
   | statisitc 3 |    float       |
   +-------------+----------------+
   | statistic 4 |    float       |
   +-------------+----------------+


   * Synchronous time intervals 1 - 900 [sec]



Dependencies
------------
* One UDP network socket is required
* A function returning actual time with 1ms precision is required

Concurrency
-----------

N/A


Limitations
-----------

N/A


Known issues and workarounds
----------------------------

N/A
