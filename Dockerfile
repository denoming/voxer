# syntax=docker/dockerfile:1

ARG BASE_CONTAINER=python:3.12-bookworm
FROM $BASE_CONTAINER

ARG USERNAME=bender
ARG USER_UID=1000
ARG USER_GID=1000
ARG ONNX_URL

RUN apt update \
 && apt install -y build-essential autoconf automake autopoint sudo vim git \
                   cmake ninja-build gdb curl tar zip unzip sudo dbus flex \
                   bison nasm texinfo wget file libpulse-dev libspdlog-dev \
                   libespeak-ng-dev espeak-ng-data libfmt-dev nlohmann-json3-dev \
                   libutfcpp-dev libsndfile1-dev

RUN groupadd -f -g $USER_GID $USERNAME \
 && useradd -l -g $USER_GID -G sudo --uid $USER_UID -ms /bin/bash $USERNAME \
 && echo $USERNAME:$USERNAME | chpasswd \
 && echo $USERNAME 'ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/010_$USERNAME || true

RUN mkdir /tmp/onnx \
 && wget -qO- $ONNX_URL | tar xz -C /tmp/onnx --strip-components=1 \
 && cd /tmp/onnx \
 && mkdir -p /usr/local/include/onnxruntime \
 && cp -pr include/* /usr/local/include/onnxruntime \
 && cp -pr lib/* /usr/local/lib \
 && cd $HOME \
 && rm -rf /tmp/onnx
