====================================
!!! THE DOCUMENTAION IS OUTDATED !!!
====================================


==============================
Proximetry Cloud Agent Example
==============================

Hardware setup
--------------

* Atmel SAMD21 Xplained Pro
* ATWINC500 Xplained Pro (Ext1)
* I/O1 Xplained Pro      (Ext2)

or

* Atmel SAMW25 Xplained Pro
* I/O1 Xplained Pro      (Ext1)

Application
-----------

The Proximetry Cloud Agent Example shows how to integrate the Proximetry Cloud Agent Component into a new application. The example implements functions responsible for reporting statistic, configuration parameters and alerts to the Cloud Portal.
The Proximetry Cloud Agent Example implements the following functionality:

**configuration parameters**

* parameter 1 - MAC address of the ATWINC1500 wireless card  (read only),
* parameter 2 - IP address of the ATWINC1500 wireless card  (read only),
* parameter 3 - SAMD21 LED0 state (0 - off, 1 - on),
* parameter 4 - SAMD21 LED0 blinking frequency.

**statistics**

* statistic 1 - I/O1 light sensor value - illuminance,
* statistic 2 - SAMD21 SW0 state,
* statistic 3 - I/O1 temperature sensor value - temperature,
* statistic 2 - ATWINC1500 RSSI.

**alerts**

* alert 1     - I/O1 temperature alert,
* alert 2     - SAMD21 SW0 alert, triggered when the SW0 button is pressed.

The values of configuration parameters, statistics and alerts, which have to be reported to the Cloud Portal, must be handed over to the Proximetry Cloud Agent Component using the exposed API.

**configuration parameters**

Parameter values are handled during the Proximetry Cloud Agent Component initialization using the `prox_agent_set_params()` function (1). Each parameter change must be committed using the `amp_agent_param_set()` function.
When parameters are changed from the Cloud Portal Web UI the `prox_agent_param_change()` (4) function is called. Implementation of particular action or configuration change can be done based on a parameter id.

**statistics**

Statistic values are updated in the `prox_agent_stats_update()` (5) function. This function should be registered as a callback during the Proximetry Cloud Agent Component initialization. The Proximetry Cloud Agent Component calls this function periodically based on the configured reporting interval value.

**alerts**

Alerts are updated in the `prox_agent_alerts_update()` (6) function. This function is called by the `prox_cloud_agent_thread_nb()` (2) function from the main application thread. The Proximetry Cloud Agent Component API provides an interface to update alerts asynchronously.
 
**agent main routines**

1) `prox_cloud_agent_init()`       - agent initialization
2) `prox_cloud_agent_thread_nb()`  - agent thread function
3) `prox_agent_set_params()`       - agent write parameters
4) `prox_agent_param_change()`     - agent parameters has been callback
5) `prox_agent_stats_update()`     - agent statistic update callback
6) `prox_agent_alerts_update()`    - agent alerts update function


The Proximetry Cloud Agent Example consist of:

* Proximetry Cloud Agent Component,
* Agent module,
* Main application.

The Agent module is responsible for retrieving configuration parameters, statistics and alerts and handing it over to the Proximetry Cloud Agent Component.
The Agent module is implemented in the following files:

* `prox_agent.c`
* `prox_agent_helper.c`.

Interface between the Proximetry Cloud Agent Component and the Agent module is defined in the `prox_agent_amp.h` header.
The Main application implements all remaining functions of the application like communication, timing etc.  Interface between the Agent module and the Main application is defined in the `prox_agent.h` header.

Dependencies
------------
* WINC1500 WiFi Network Controller driver,
* STDIO Redirect, printf to specified io,
* Proximetry Cloud Agent Component,
* Task timer functionality using a TC peripheral,
* Universal Asynchronous receiver/transmitter (UART) communication in synchronous/blocking mode,
* Analog to digital converter (ADC) in synchronous/blocking mode,
* I2C Master driver in synchronous/blocking mode,
* Delay functionality using SysTick,
* Serial Peripheral Interface (SPI) master communication in synchronous/blocking mode.

Known issues and workarounds
----------------------------

N/A


