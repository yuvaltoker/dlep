#!/bin/bash

if [ "$IMPLEMENTATION" = "modem" ]; then
    /adjusted-dlep/build/Dlep config-file /adjusted-dlep/config/modem.xml
elif [ "$IMPLEMENTATION" = "router" ]; then
    /adjusted-dlep/build/Dlep config-file /adjusted-dlep/config/router.xml
fi
