#!/bin/bash

command_string="/adjusted-dlep/build/Dlep config-file /adjusted-dlep/config/"

# checks whether the implementation is for modem of for router 
if [ "$IMPLEMENTATION" = "modem" ]; then
	command_string+="/modem.xml"
elif [ "$IMPLEMENTATION" = "router" ]; then
	command_string+="/router.xml"
fi


#####################################################################
# is it the right syntax for more than one configuration variable???#
#####################################################################
# adding discovry interface:
command_string+=" discovery-iface ${DISCOVERY_IFACE}"


# do we have discovery interval env variable?:
if [ -z "$DISCOVERY_INTERVAL" ]; then
	command_string+=" discovery-interval ${DISCOVERY_INTERVAL}"
fi


# do we have heartbeat interval env variable?:
if [ -z "$HEARTBEAT_INTERVAL" ]; then
	command_string+=" heartbeat-interval ${HEARTBEAT_INTERVAL}"
fi


eval $command_string

#if [ "$IMPLEMENTATION" = "modem" ]; then
#    /adjusted-dlep/build/Dlep config-file /adjusted-dlep/config/modem.xml discovery-iface $DISCOVERY_IFACE
#elif [ "$IMPLEMENTATION" = "router" ]; then
#    /adjusted-dlep/build/Dlep config-file /adjusted-dlep/config/router.xml discovery-iface $DISCOVERY_IFACE
#fi
