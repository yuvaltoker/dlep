#!/bin/bash

if [ "$IMPLEMENTATION" = "modem" ]; then
    /adjusted-dlep/build/Dlep config-file /adjusted-dlep/config/modem.xml discovery-iface $DISCOVERY_IFACE
elif [ "$IMPLEMENTATION" = "router" ]; then
    /adjusted-dlep/build/Dlep config-file /adjusted-dlep/config/router.xml discovery-iface $DISCOVERY_IFACE
fi
