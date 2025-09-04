# Voxer

[![CI](https://github.com/denoming/voxer/actions/workflows/build.yaml/badge.svg)](https://github.com/denoming/voxer/actions/workflows/build.yaml)
[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

## Prerequisites

| Name          | Source | Component |
|---------------|--------|-----------|
| espeak-ng     | host   | core      |
| onnx-runtime  | host   | core      |
| fmt           | vcpkg  | core      |
| spdlog        | vcpkg  | core      |
| nlohmann-json | vcpkg  | core      |
| utfcpp        | vcpkg  | core      |
| uni-algo      | vcpkg  | core      |
| libsndfile    | vcpkg  | audio     |
| cxxopts       | vcpkg  | CLI       |

Install espeak-ng:

```shell
$ sudo apt install libespeak-ng-dev espeak-ng-data 
```

Install ONNX (GPU):

```shell
$ export DEST_DIR=$HOME/.local
$ wget https://github.com/microsoft/onnxruntime/releases/download/v1.21.0/onnxruntime-linux-x64-gpu-1.21.0.tgz
$ tar xf onnxruntime-linux-x64-gpu-1.21.0.tgz -C /tmp
$ pushd /tmp/onnxruntime-linux-x64-gpu-1.21.0
$ mkdir -p $DEST_DIR/{include/onnxruntime,lib}
$ cp -r include/* $DEST_DIR/include/onnxruntime
$ cp -r lib/* $DEST_DIR/lib
$ popd
$ rm -f onnxruntime-linux-x64-gpu-1.21.0.tgz
```

Install ONNX (CPU, mutually exclusive with GPU variant, see above):

```shell
$ export DEST_DIR=$HOME/.local
$ wget https://github.com/microsoft/onnxruntime/releases/download/v1.21.0/onnxruntime-linux-x64-1.21.0.tgz
$ tar xf onnxruntime-linux-x64-1.21.0.tgz -C /tmp
$ pushd /tmp/onnxruntime-linux-x64-1.21.0
$ mkdir -p $DEST_DIR/{include/onnxruntime,lib}
$ cp -r include/* $DEST_DIR/include/onnxruntime
$ cp -r lib/* $DEST_DIR/lib
$ popd
$ rm -f onnxruntime-linux-x64-1.21.0.tgz
````

## Introduction

**Voxer** is a tiny text to speech C++ library.

| Type  | Name                     | Stage   | Default |  Type   | Example                                         | Description                               |
|-------|--------------------------|---------|:-------:|:-------:|-------------------------------------------------|-------------------------------------------|
| cmake | `VOXER_ENABLE_TESTS`     | Build   |   OFF   | Boolean |                                                 | Enable unit tests                         |
| cmake | `VOXER_ENABLE_FORMATTED` | Build   |   ON    | Boolean |                                                 | Enable formatted audio data support       |
| env   | `VOXER_VOICES_DOWNLOAD`  | Build   |   OFF   | Boolean |                                                 | Enable voices downloading while building  |
| env   | `VOXER_VOICES_LIST`      | Build   |    -    |  List   | "en_US+amy+medium;uk_UA+lada+x_low"             | Set the list of voices to download        |
| env   | `VOXER_MODEL_FILE`       | Runtime |    -    | String  | "/usr/share/voxer/voices/en_US-amy-medium.onnx" | Set the path to voice model file (*.onnx) |
| env   | `VOXER_ESPEAK_DIR`       | Runtime |    -    | String  | "/usr/lib/espeak-ng-data"                       | Set The path to eSpeak-Ng files           |

See the list of available voices to download at `config/voices/voices.csv` file.

## Building

Output artifacts default location is `<build-dir>/stage`.

Packing artifacts includes the following files:
* `libvoxer_<version>_.deb`
* `libvoxer-dev_<version>_.deb`
* (optional) `libvoxer-data_<version>_.deb`

### By Docker (recommended)

Run docker container:
```shell
$ bash scripts/run-build-env.sh -m
```

Define variable whether to download voice models:
```shell
# Run to enable voice models downloading
export VOXER_VOICES_DOWNLOAD=1
export VOXER_VOICES_LIST="en_US+amy+medium;uk_UA+lada+x_low"
# Run to disable voice models downloading
unset VOXER_VOICES_DOWNLOAD
unset VOXER_VOICES_LIST
```

Build and create packages:
```shell
# Debug
$ cmake --workflow --fresh --preset debug-docker
# Release
$ cmake --workflow --fresh --preset release-docker
```

### Locally

Define variable whether to download voice models:
```shell
# Run to enable voice models downloading
export VOXER_VOICES_DOWNLOAD=1
export VOXER_VOICES_LIST="en_US+amy+medium;uk_UA+lada+x_low"
# Run to disable voice models downloading
unset VOXER_VOICES_DOWNLOAD
unset VOXER_VOICES_LIST
```

Build and create packages:
```shell
# Debug
$ cmake --workflow --fresh --preset debug
# Release
$ cmake --workflow --fresh --preset release
```

## Installing

Install artifacts to specific location:

```shell
$ cmake --install <build-dir> --prefix <destination-path>/voxer
```

## Usage

### By vcpkg package manager

* Add `voxer` dependency into project `vcpkg.json` file
* Download `*.onnx` and `*.onnx.json` model files (see [VOICES.md](VOICES.md) file)

### By CLI tool

Define input parameters:

```shell
export MODEL="<model>.onnx"
export FILES="/usr/share/espeak-ng-data"
export INPUT="Hello World"
```

Display version:

```shell
$ voxer-cli -v
Voxer 0.2.2 (447a7fb9)
```

Display information:

```shell
$ voxer-cli --model $MODEL --files $FILES --info
```

Save into WAV file:

```shell
$ echo $INPUT | voxer-cli --model $MODEL --files $FILES --output-file welcome.wav --wav
```

Save into RAW file:

```shell
$ echo $INPUT | voxer-cli --model $MODEL --files $FILES --output-file welcome.raw --raw
```

Play using gstreamer (WAV format):

```shell
$ echo $INPUT \
 | voxer-cli --model $MODEL --files $FILES --wav --output-file - \
 | gst-launch-1.0 -e fdsrc fd=0 ! wavparse ! audioconvert ! audioresample ! autoaudiosink sync=false
```

Play using gstreamer (RAW format):

```shell
$ echo $INPUT \
 | voxer-cli --model $MODEL --files $FILES --raw --output-file - \
 | gst-launch-1.0 -e fdsrc fd=0 ! rawaudioparse num-channels=1 sample-rate=22050 ! audioconvert ! audioresample ! autoaudiosink sync=false
```
