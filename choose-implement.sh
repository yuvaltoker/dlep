#!/bin/bash

# adjusting dlep-draft-8175.xml by the extensions from enviroment variables:

# bla bla/usr/local/etc/dlep/dlep-rfc-8175.xml - where the file will be eventually
# bla bla adjusted-dlep/config/protocol/dlep-rfc-8175.xml - where the file in the container
# bla bla adjusted-LL-DLEP/adjusted-DLEP/config/protocol/dlep-rfc-8175.xml - where the file in host

#if [ "$EXT_PAUSE" = "true" ]; then
sed -i "/<!-- <xi:include href=\"pause-extension-8651.xml\"\/> -->/c\    <xi:include href=\"pause-extension-8651.xml\"/>" ./adjusted-dlep/config/protocol/dlep-rfc-8175.xml
#fi

if [ "$EXT_LID" = "true" ]; then
	sed -i "/<!-- <xi:include href=\"lid-extension-8703.xml.xml\"\/> -->/c\    <xi:include href=\"lid-extension-8703.xml.xml\"/>" ./adjusted-dlep/config/protocol/dlep-rfc-8175.xml
fi

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