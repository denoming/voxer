#!/usr/bin/env bash

# Copyright 2025 Denys Asauliak
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

ONNX_URL=https://github.com/microsoft/onnxruntime/releases/download/v1.21.0/onnxruntime-linux-x64-1.21.0.tgz

apt update
apt install -y build-essential autoconf automake autopoint sudo vim git \
               cmake ninja-build gdb curl tar zip unzip sudo dbus flex \
               bison nasm texinfo wget file libpulse-dev libspdlog-dev libcxxopts-dev \
               libespeak-ng-dev espeak-ng-data libfmt-dev nlohmann-json3-dev \
               libutfcpp-dev libsndfile1-dev clang-format-16 libgtest-dev \
               libgmock-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
               gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad

mkdir /tmp/onnx
wget -qO- $ONNX_URL | tar xz -C /tmp/onnx --strip-components=1
cd /tmp/onnx
mkdir -p /usr/local/include/onnxruntime
cp -pr include/* /usr/local/include/onnxruntime
cp -pr lib/* /usr/local/lib
cd $HOME
rm -rf /tmp/onnx
