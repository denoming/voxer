# syntax=docker/dockerfile:1

ARG PLATFORM=arm64v8
FROM $PLATFORM/debian:bookworm

ARG USER_NAME=bender
ARG USER_UID=1000
ARG USER_GID=1000

ENV VCPKG_ROOT="/home/$USER_NAME/.vcpkg"
ENV VCPKG_FORCE_SYSTEM_BINARIES=true
ENV VCPKG_DISABLE_METRICS=true

USER root

RUN apt update \
 && apt install -y build-essential autoconf automake autopoint sudo vim git \
                   cmake ninja-build gdb curl tar zip unzip sudo dbus flex \
                   bison nasm texinfo wget file libpulse-dev

# Create custom user
RUN groupadd -f -g $USER_GID $USER_NAME \
 && useradd -l -g $USER_GID -G sudo --uid $USER_UID -ms /bin/bash $USER_NAME \
 && echo $USER_NAME:$USER_NAME | chpasswd \
 && echo $USER_NAME 'ALL=(ALL) NOPASSWD:SETENV: ALL' > /etc/sudoers.d/010_$USER_NAME || true

USER $USER_NAME

RUN wget -O /tmp/vcpkg.zip https://github.com/microsoft/vcpkg/archive/refs/tags/2024.09.30.zip \
 && unzip -q /tmp/vcpkg.zip -d /tmp/vcpkg \
 && mv /tmp/vcpkg/vcpkg-2024.09.30 $VCPKG_ROOT \
 && rm -rf /tmp/vcpkg* \
 && bash -c $VCPKG_ROOT/bootstrap-vcpkg.sh -disable-metrics \
 && mkdir -p $HOME/.local/bin \
 && ln -s $VCPKG_ROOT/vcpkg $HOME/.local/bin/vcpkg

CMD ["/bin/bash","--rcfile","$HOME/.profile"]
