=========================================
Proximetry Cloud Agent Component - Alerts
=========================================

Alerts are a Cloud Data Type utilized to signalize abnormal states to the Cloud Service,

API
---
To set/clear alerts in the Cloud Service the

`prox_alert_set/clear__<name>()`

functions, from `prox_agent.h` API, must be used.
The <name> is the name of alert defined during component configuration process.

E.g if a name, of one of the alert defined for the Cloud Agent, is "temperature" then
the API contains `prox_alert_set__temperature()` and  `prox_alert_clear__temperature()` functions to set and clear the alert respectively, in the Cloud Service.


Configuration
-------------
* name
    Name of the alert which is used in definitions of Application API functions.
* ID
    Defines identifiers for Configuration Parameter Proximetry Data Type. The Proximetry Data Type IDs must match definition in the Proximetry Portal.
