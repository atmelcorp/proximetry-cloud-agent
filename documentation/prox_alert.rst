=========================================
Proximetry Cloud Agent Component - Alerts
=========================================

Alerts are used to monitor device health and critical parameters. Once a predefined alert threshold is crossed the Agent reports it to the Proximetry Portal.
Each alert can either be in the CLEAR or in the SET state. An example alert can be Low Fuel notification in a car.

API
---
To set/clear alerts in the Proximetry Portal use the

`prox_alert_set/clear__<name>()`

function, from `prox_agent.h` API.
The <name> is an Alert name defined during the component configuration process.

E.g. for a "temperature" Alert the Application API provides the following functions:

* `prox_alert_set__temperature()` to send an Alert to the Proximetry Portal.
* `prox_alert_clear__temperature()` to clear the Alert.


Configuration
-------------
* name
    Name of an Alert.
* ID
    Defines a unique identifier of an Alert. The ID must match the Alert definition in the Proximetry Portal.
