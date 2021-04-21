#!/bin/bash
echo $(git rev-parse HEAD) >> sourceHash.txt
docker build --tag $1 --file dockerfiles/build-dlep-enviroment-dockerfile .
