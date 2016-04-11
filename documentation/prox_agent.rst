================================
Proximetry Cloud Agent Component
================================

The Proximetry Cloud Agent Component contains a library and API to accelerate development of IoT applications.

The Proximetry Cloud Agent Library provides the following functionality:

1. Secure two-way communication with the Cloud Portal
2. Automatic device registration under specific Cloud Portal account
3. An option to report device sensor values with a configurable reporting interval
4. An option to monitor device health and report alerts
5. An option to synchronize device configuration parameters and modify them remotely from the Cloud Portal

**Cloud Data Types**

The Proximetry Cloud Agent Component allows to use the following types of information to build IoT application:

* Statistics,
* Alerts,
* Configuration parameters.

Statistics are data periodically reported by the application to the Cloud Portal. An example statistic can be a temperature or humidity value reported by attached sensors.
Alerts are used to monitor device health and critical parameters. Once a predefined alert threshold is crossed the Agent reports it to the Cloud Portal. Each alert can either be in the CLEAR or in the SET state. An example alert can be Low Fuel notification in a car.
Configuration parameters are user-defined settings that controls various aspects of application behavior.  An example configuration parameter can be “Light Control” allowing to turn on or off a light bulb.
Supported types of particular information are defined below.

Statistics:  *bool, int8, int16, int32, uint8, uin16, uin32, float, string*

Alerts:  binary values, which can be *set* or *clear*.

Configuration parameters: *bool, int8, int16, int32, uint8, uin16, uin32, float, string*.

The information type determines how the information is presented in the Cloud Portal Web UI. Statistics are displayed as time series on charts, alerts are displayed as notifications to the operator and configuration parameters are displayed on an editable form.

**Communication**

Proximetry Cloud Agent Component requires UDP transport layer for two-way communication with the Cloud Portal. The default port used to communicate with the Cloud Portal is UDP 5050.
Note that some firewalls may block UDP communication. Please contact with your IT department in case you experience any connectivity issues with the Cloud Portal.

**Proximetry Cloud Agent Component Configuration**

* Activation code:
    Device activation code is a 74-char length alphanumeric string assigned to your Cloud Portal account. The device activation code can be found under “Provisioning->Device Activation Codes” in the Cloud Portal main menu.
* Device name
    An initial device name string that will be reported to the Cloud Portal for device identification. This string can be used in the search field to quickly find a device in the Cloud Portal.
    Note that the device name can be modified by the Operator from the Cloud Portal Web UI be the name.

Features
--------

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
