#!/bin/bash

# adjusting dlep-draft-8175.xml by the extensions from enviroment variables:
sed -i "/^modem_name=/c\modem_name='$container_name'" ./modem/dock_modem/modem.py

sed -i "/^<!-- <xi:include href=\"pause-extension-8651.xml\"/> -->/c\<xi:include href=\"pause-extension-8651.xml\"/>" 

command_string="/adjusted-dlep/build/Dlep config-file /adjusted-dlep/config/"

# checks whether the implementation is for modem of for router 
if [ "$IMPLEMENTATION" = "modem" ]; then
	command_string+="modem.xml"
elif [ "$IMPLEMENTATION" = "router" ]; then
	command_string+="router.xml"
fi

command_string+=" discovery-iface $DISCOVERY_IFACE"


# do we have discovery interval env variable?:
if [ ! -z "$DISCOVERY_INTERVAL" ]; then
	command_string+=" discovery-interval $DISCOVERY_INTERVAL"
fi


# do we have heartbeat interval env variable?:
if [ ! -z "$HEARTBEAT_INTERVAL" ]; then
	command_string+=" heartbeat-interval $HEARTBEAT_INTERVAL"
fi

echo "$command_string"
eval "$command_string"


