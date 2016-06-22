================================
Proximetry Cloud Agent Component
================================

The Proximetry Cloud Agent Component allows to connect a device to the Proximetry Portal.
The component provides the following functionality:

1. Automatic registration with the Proximetry Portal
2. An option to monitor device state, attached sensors and alerts
3. Remote control from the Proximetry Portal Web Interface

**Cloud's Data Types**

Proximetry Portal differentiates three types of information:

* Statistics,
* Alerts,
* Configuration parameters.

**Statistic** parameters are data periodically reported by the application to the Proximetry Portal. An example statistic can be a temperature or humidity value reported by attached sensors.

**Alerts** are used to monitor device health and critical parameters. Once a predefined alert threshold is crossed the Agent reports it to the Proximetry Portal. Each alert can either be in the CLEAR or in the SET state. An example alert can be Low Fuel notification in a car.

**Configuration parameters** are user-defined settings that controls various aspects of application behavior.  An example configuration parameter can be “Light Control” allowing to turn on or off a light bulb.
Supported types of particular information are defined below.

Statistics:  *bool, int8, int16, int32, uint8, uin16, uin32, float, string*

Alerts:  binary values, which can be *set* or *clear*.

Configuration parameters: *bool, int8, int16, int32, uint8, uin16, uin32, float, string*.

The information type determines how the information is presented in the Proximetry Portal Web UI. Statistics are displayed as time series on charts, alerts are displayed as notifications to the operator and configuration parameters are displayed on an editable form.

To get more details of Cloud Data Types please look into User Guide for Cloud Data Components.


**Communication**

Proximetry Cloud Agent uses UDP transport layer for two-way communication with the Proximetry Portal. The default port used to communicate with the Proximetry Portal is UDP 5050.
Note that some firewalls may block UDP communication. Please contact with your IT department in case you experience any connectivity issues with the Proximetry Portal.

**Proximetry Cloud Agent Component Design**

The Proximetry Cloud Agent Component consists of a root Agent Component and three subcomponents, one for each Cloud's Data Type.
The Proximetry Cloud Agent Component API is defined in two files:

* `prox_agent/core/prox_core.h`   - System Interface provides an API to initialize the Proximetry Agent Component,
* `prox_agent/prox_agent.h`       - Application Interface provides API to implement the application logic.


Integration
-----------

To integrate the Proximetry Cloud Agent Component into a new project the following steps must be taken:

1. Integrate the network related API into the project code.
   The agent component provides two network related functions:

   * `prox_agent_get_socket()`,
   * `prox_agent_recv_msg()`.

   This functions must be integrated with network driver in the project.
2. Integrate the component initialization and task function into the project:

   * `prox_agent_init()`,
   * `prox_agent_task()`.

3. Use functions and implement callbacks provided by the Application Interface API to handle Cloud Data Types.

(Please see the API descriptions in the prox_core.h and prox_agent.h files and look at the Cloud Agent existing example provided for Atmel Start).

Configuration
-------------

To get Cloud Agent working the following configuration must be set:

* Device model id: PROX_DEVICE_MODEL
* Device model version: PROX_MODEL_VER
    Device model id and Device model version uniquely identifies Device Model and its
    capabilities (Configuration Parameters, Statistics and Alerts) defined in the Proximetry Portal. The provided Device Model definition must exist on the Proximetry Porta.
* Activation code: PROX_ACTIVATION_CODE
    Device activation code is a 74-char length alphanumeric string assigned to your Proximetry Portal account. The device activation code can be found under “Provisioning->Device Activation Codes” in the Proximetry Portal main menu.
* Device name: PROX_DEVICE_NAME
    An initial device name string that will be reported to the Proximetry Portal for device identification. This string can be used in the search field to quickly find a device in the Proximetry Portal.
    Note that the device name can be modified by the Operator from the Proximetry Portal Web UI be the name.

Features
--------

   * Secured communication with the Proximetry Portal
   * Sensors monitoring with configurable reporting intervals (1 - 900 [sec])    
   * Remote control 
   * Alerts   

Dependencies
------------
* One UDP network socket is required, 
* A function returning actual time with 1ms precision is required,
* STDIO for logging.

Concurrency
-----------

N/A


Limitations
-----------

N/A


Known issues and workarounds
----------------------------

N/A
