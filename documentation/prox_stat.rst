=============================================
Proximetry Cloud Agent Component - Statistics
=============================================

Statistics are data periodically reported by the application to the Proximetry Portal.
Frequency of reports to the Portal is defined by the Periodic Message Reporting Interval.
Statistics are presented as time series on graphical charts in the Proximetry Portal Web UI.
An example statistic can be a temperature or humidity value reported by the attached sensors.

API
---
Statistic data are automatically updated by the Cloud Agent. For getting a Statistic value the Agent Module utilizes callbacks declared in `prox_agent/prox_agent.h`:

`prox_callback__get_<name>()`

The <name> is the name of statistic defined during a component configuration process.
Statistics callbacks must be implemented in the application.
Note that if the callback is not implemented in the application, the Proximetry Cloud Agent module uses a default weak handler which returns statistic value 0.

E.g. for an "rssi" Statistic the Application API declares the following callback:

`float prox_callback__get_rssi(void)`

to get RSSI Statistic value.


Configuration
-------------
* name
     Name of a Statistic
* type
    Type of the Statistic.
* ID
    Defines a unique identifier of a Statistic. The ID must match the Statistic definition in the Proximetry Portal.
