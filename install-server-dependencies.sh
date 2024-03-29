#!/bin/bash

function install_served_dependencies() {
  rm -rf served
  git clone https://github.com/evanugarte/served.git
  apt-get install -y libboost-all-dev
}

function install_json_dependencies() {
  if ! [[ -d "SimpleJSON" ]]; then
      mkdir SimpleJSON
  fi
  cd SimpleJSON
  curl https://raw.githubusercontent.com/nbsdx/SimpleJSON/master/json.hpp -o json.hpp
}

function main() {
  cd lib
#install_served_dependencies
  install_json_dependencies
  cd ..
}

main