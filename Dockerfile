# syntax=docker/dockerfile:1

ARG PLATFORM=arm64v8
FROM $PLATFORM/debian:bookworm

ARG USER_NAME=bender
ARG USER_UID=1000
ARG USER_GID=1000
ARG ONNX_URL

ENV VCPKG_ROOT="/home/$USER_NAME/.vcpkg"
ENV VCPKG_FORCE_SYSTEM_BINARIES=true
ENV VCPKG_DISABLE_METRICS=true

USER root

RUN apt update \
 && apt install -y build-essential autoconf automake autopoint sudo vim git \
                   cmake ninja-build gdb curl tar zip unzip sudo dbus flex \
                   bison nasm texinfo wget file libpulse-dev \
                   libespeak-ng-dev espeak-ng-data

# Create custom user
RUN groupadd -f -g $USER_GID $USER_NAME \
 && useradd -l -g $USER_GID -G sudo --uid $USER_UID -ms /bin/bash $USER_NAME \
 && echo $USER_NAME:$USER_NAME | chpasswd \
 && echo $USER_NAME 'ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/010_$USER_NAME || true

# Install ONNX Runtime Library
RUN mkdir /tmp/onnx \
 && wget -qO- $ONNX_URL | tar xz -C /tmp/onnx --strip-components=1 \
 && cd /tmp/onnx \
 && mkdir -p /usr/local/include/onnxruntime \
 && cp -pr include/* /usr/local/include/onnxruntime \
 && cp -pr lib/* /usr/local/lib \
 && cd $HOME \
 && rm -rf /tmp/onnx

USER $USER_NAME

# Install vcpkg
RUN wget -O /tmp/vcpkg.zip https://github.com/microsoft/vcpkg/archive/refs/tags/2025.01.13.zip \
 && unzip -q /tmp/vcpkg.zip -d /tmp/vcpkg \
 && mv /tmp/vcpkg/vcpkg-2025.01.13 $VCPKG_ROOT \
 && rm -rf /tmp/vcpkg* \
 && bash -c $VCPKG_ROOT/bootstrap-vcpkg.sh -disable-metrics \
 && mkdir -p $HOME/.local/bin \
 && ln -s $VCPKG_ROOT/vcpkg $HOME/.local/bin/vcpkg

CMD ["/bin/bash","--rcfile","$HOME/.profile"]
