==============================
Atmel Cloud Proximetry Example
==============================

Hardware setup
--------------

* Atmel SAMD21 Xplained Pro
* ATWINC500 Xplained Pro (Ext1)
* I/O1 Xplained Pro      (Ext2)

or

* Atmel SAMW25 Xplained Pro
* I/O1 Xplained Pro      (Ext1)

Configuration
-------------
To get a fully functional project the following parameters should be defined:

**Proximetry Cloud Agent Component**

* Activation code,
* Device name.

**Atmel Cloud Proximetry Example**

* Network SSID
* Network password,
* Security type.


Application
-----------

The Atmel Cloud Proximetry Example shows how to create an IoT application using the Proximetry Cloud Agent Component. The example implements functions responsible for reporting statistic, configuration parameters and alerts to the Cloud Portal.
The Atmel Cloud Proximetry Example implements the following functionality:

**configuration parameters**

* parameter 1 - MAC address of the ATWINC1500 wireless card  (read only),
* parameter 2 - IP address of the ATWINC1500 wireless card  (read only),
* parameter 3 - SAMD21 LED0 state (0 - off, 1 - on),
* parameter 4 - SAMD21 LED0 blinking frequency in Hz.

Configuration parameters are read from a device using the `prox_agent_set_params()` function (1). Each modification of configuration parameters must be followed by the `amp_agent_param_set()` function. The `amp_agent_param_set()` triggers configuration parameters synchronization with the Cloud Portal.
When a configuration parameter is changed from the Cloud Portal the `prox_agent_param_change()` (4) function is called. Implementation of particular action on configuration parameter change event can be done based on the configuration parameter id.

**statistics**

* statistic 1 - I/O1 light sensor value - illuminance in Lux,
* statistic 2 - SAMD21 SW0 state (0 - off, 1 - on),
* statistic 3 - I/O1 temperature sensor value - temperature in Celsius ,
* statistic 2 - ATWINC1500 RSSI.

Statistic values are updated in the `prox_agent_stats_update()` (5) function. This function should be registered as a callback during the Proximetry Cloud Agent Module initialization. The Proximetry Cloud Agent Library calls this function periodically based on the configured reporting interval value.

**alerts**

* alert 1 - I/O1 temperature alert.
            Threshold levels:
               * temperature > 29.0 [C] - alert on
               * temperature < 28.0 [C] - alert off

* alert 2 - SAMD21 SW0 alert, triggered when the SW0 button is pressed.

Alerts are updated in the `prox_agent_alerts_update()` (6) function. This function is called by the `prox_agent_thread_nb()` (2) function from the main application thread. The Proximetry Cloud Agent Library provides an interface to update alerts asynchronously.

The values of configuration parameters, statistics and alerts, which have to be reported to the Cloud Portal, must be handed over to the Proximetry Cloud Agent Library using the exposed API.

**agent main routines**

1) `prox_agent_init()`       - agent initialization
2) `prox_agent_thread_nb()`  - agent thread function
3) `prox_agent_set_params()`       - agent write parameters
4) `prox_agent_param_change()`     - agent parameters has been changed callback
5) `prox_agent_stats_update()`     - agent statistic update callback
6) `prox_agent_alerts_update()`    - agent alerts update function


The Proximetry Cloud Agent Example consist of:

* Proximetry Cloud Agent Library (AtmelStart component),
* Cloud Agent Module,
* Main application.

The Atmel Cloud Proximetry Example implements Agent module functions responsible for retrieving information from a device and handing over configuration parameters, statistics and alerts to the API provided by the Proximetry Cloud Agent Component API.
Above functionality is implemented in the following files:

* `prox_agent.c`
* `prox_agent_amp.c`.

The main application implements all the remaining application functions like communication or timing. Interface between the Agent module and the main application is defined in the `prox_agent_amp.h` file.

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


