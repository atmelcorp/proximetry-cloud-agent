=========================================
Proximetry Cloud Agent Component - Params
=========================================

Configuration parameters are user-defined settings that controls various aspects of the application behavior.
Configuration Parameters can be controlled from the Proximetry Portal Web UI.  
An example configuration parameter can be “Light Control” allowing to turn on or off a light bulb.


API
---
To push the Configuration Parameter value to the Proximetry Portal use the

`prox_conf_param_push__<name>()`

function from `prox_agent.h` API.
The <name> is a Configuration Parameter name defined during the component configuration process.

Use the `prox_conf_param_push__<name>()` function to:
•	initialize Configuration Parameters in the Proximetry Portal at the application startup 
•	notify the Proximetry Portal about any local configuration changes  (e.g. when a new IP address is obtained from DHCP)

To handle a Configuration Parameter change request received from the Proximetry Portal implement the following callback function:

`prox_callback__set_<name>()`

declared in the `prox_agent/prox_agent.h`.
The <name> is a Configuration Parameter name defined during the component configuration process.
Note that if the callback is not implemented the Proximetry Cloud Agent module uses a default, dummy handler.

E.g for an "ip_address" Configuration Parameter the Application API provides the following functions:

* `prox_conf_param__push_ip_address(char *ip_address)` to push an IP Address value to the Proximetry Portal
* `prox_callback__set_ip_address(char *ip_address)` to handle IP Address change request from the Proximetry Portal 

 
Configuration
-------------
* name
     Name of a Configuration Parameter
* type
    Type of the Configuration Parameter.
* ID
    Defines a unique identifier of a Configuration Parameter. The ID must match the Configuration Parameter definition in the Proximetry Portal.
