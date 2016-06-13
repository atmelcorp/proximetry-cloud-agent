=========================================
Proximetry Cloud Agent Component - Params
=========================================

Configuration Parameters are a Cloud Data Type which values can be reported to the Cloud Service,
and which values can be set from the Cloud Service Gui. Configuration Parameters can be used to control the device behavior.


API
---
To updated the Configuration Parameters value in the Cloud Service the

`prox_conf_param_push__<name>()`

functions from `prox_agent.h` api must be used.
The <name> is a name of parameter defined during component configuration process.

E.g if a name of one of the configuration parameters defined for the Cloud Agent
is "ip" (type string) then the API contains `prox_conf_param__push_ip(char *ip)` function for update this parameter in the Cloud Service.

To handle the Configuration Parameter change, from the Clud Service, the

`prox_callback__set_<name>()`

callback is utilize by the cloud agent module.The callback is declared in `prox_agent/prox_agent.h`.
When value, for particualar configuration paramiter
arrives this callback is called.
The <name> is a name of parameter defined during component configuration process.
These callbacks have to be defined in the project by user.  If the callback is not defined then Cloud Agent module uses default, dummy, weak handler for particular param. 

Configuration
-------------
* name
    Name of the configuration parameter which is used in definitions of Application API functions.
* type
    Type of configuration parameters.
* ID
    Defines identifiers for Configuration Parameter Proximetry Data Type. The Proximetry Data Type IDs must match definition in the Proximetry Portal.
