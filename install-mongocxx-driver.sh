#!/bin/bash

# i found the below steps from:
# https://developer.mongodb.com/community/forums/t/problems-trying-to-compile-mongocxx/2084
function install_mongocxx_dependencies() {
  TOP_DIR=$(pwd)
  apt-get install -y libmongoc-1.0-0 libbson-1.0 cmake libssl-dev libsasl2-dev zlib1g-dev

# changing 1.17.0 (mongocxx-3.6) into 1.22.1 (mongocxx-3.7)
  wget https://github.com/mongodb/mongo-c-driver/releases/download/1.22.1/mongo-c-driver-1.22.1.tar.gz
  tar xzf mongo-c-driver-1.22.1.tar.gz
  cd mongo-c-driver-1.22.1
  mkdir cmake-build
  cd cmake-build
  cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
  make install

  git clone https://github.com/mongodb/mongo-cxx-driver.git \
      --branch releases/stable --depth 1
  cd mongo-cxx-driver/build

  cmake .. \
      -DCMAKE_BUILD_TYPE=Release \
      -DBSONCXX_POLY_USE_MNMLSTC=1 \
      -DCMAKE_INSTALL_PREFIX=/usr/local

  make
  make install
  cd $TOP_DIR
  rm -rf mongo-c-driver-1.22.1*
}

function main() {
  install_mongocxx_dependencies
}

main