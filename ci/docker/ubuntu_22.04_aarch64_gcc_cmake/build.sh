#!/bin/sh
cd `dirname $0`
docker build -t libosmscout/ubuntu_22.04_aarch64_gcc_cmake .
