=============================================
Proximetry Cloud Agent Component - Statistics
=============================================

Statistics are a Cloud Data Type which values are reported to the Cloud Service in time intervals.
The statistics can be use to reporting the values from sensors and are displayed in time series on the Cloud Server WebGUI.

API
---
Statistic values are updated automatically by the Cloud agent module. For getting the statistics value the Agent Module utilizes

`prox_callback__get_<name>()`

callbacks which are declared in `prox_agent/prox_agent.h` file. The <name> is the name of statistic defined during a component configuration process.
These callbacks have to be defined in the project by user.  If the callback is not defined then Cloud Agent module use default weak handler for particular statistic 
which return a default value equal 0.

E.g. if a name, of one of the statistic defined for the Cloud Agent, is "rssi" (type float) then the API contains `fload prox_callback__get_rssi(void)` callback  declaration
and this callback will be use, by the Cloud Agent Module, to get the statistic's value.


Configuration
-------------
* name
    Name of the statistic which is used in definitions of Application API functions.
* type
    Type of statistic 
* ID
    Defines identifiers for Configuration Parameter Proximetry Data Type. The Proximetry Data Type IDs must match definition in the Proximetry Portal.
