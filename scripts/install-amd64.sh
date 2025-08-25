#!/usr/bin/env bash

ONNX_URL=https://github.com/microsoft/onnxruntime/releases/download/v1.21.0/onnxruntime-linux-x64-1.21.0.tgz

apt update
apt install -y build-essential autoconf automake autopoint sudo vim git \
               cmake ninja-build gdb curl tar zip unzip sudo dbus flex \
               bison nasm texinfo wget file libpulse-dev libspdlog-dev \
               libespeak-ng-dev espeak-ng-data libfmt-dev nlohmann-json3-dev \
               libutfcpp-dev libsndfile1-dev

mkdir /tmp/onnx
wget -qO- $ONNX_URL | tar xz -C /tmp/onnx --strip-components=1
cd /tmp/onnx
mkdir -p /usr/local/include/onnxruntime
cp -pr include/* /usr/local/include/onnxruntime
cp -pr lib/* /usr/local/lib
cd $HOME
rm -rf /tmp/onnx
