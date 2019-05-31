#!/bin/sh
set -e

apt-get update

# noninteractive tzdata ( https://stackoverflow.com/questions/44331836/apt-get-install-tzdata-noninteractive )
export DEBIAN_FRONTEND=noninteractive

# CI specific packages
apt-get install -y gcc git clang valgrind ccache

# Core dependencies
apt-get install -y cmake libboost-all-dev libpugixml-dev qtbase5-dev qtdeclarative5-dev qtmultimedia5-dev libqt5opengl5-dev libqcustomplot-dev

# workaround issue in pugixml
ln -s /usr/lib/x86_64-linux-gnu/libpugixml.so /lib/x86_64-linux-gnu/libpugixml.so.1.8
ln -s /usr/lib/x86_64-linux-gnu/libpugixml.a /lib/x86_64-linux-gnu/libpugixml.a
