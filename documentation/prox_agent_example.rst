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

The Proximetry Cloud Agent Example shows how to integrate the Proximetry Cloud Agent Component into a new application. The example implements functions responsible for reporting statistic, configuration parameters and alerts to the Proximetry Portal.
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

The values of configuration parameters, statistics and alerts, which have to be reported to the Proximetry Portal, must be handed over to the Proximetry Cloud Agent Component using the exposed API.


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


