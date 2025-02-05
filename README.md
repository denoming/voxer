# Voxer

Dependencies:

* espeak-ng (library, headers and data files)
* (vcpkg) fmt
* (vcpkg) spdlog
* (vcpkg) nlohmann-json
* (vcpkg) utfcpp
* (vcpkg) uni-algo

Options dependencies (formatted, `VOXER_ENABLE_FORMATTED` cmake option):

* (vcpkg) libsndfile

Options dependencies (CLI enabled, `VOXER_ENABLE_CLI` cmake option):

* (vcpkg) cxxopts

## Prerequisites

Install espeak-ng:

```shell
$ sudo apt install libespeak-ng-dev espeak-ng-data 
```

Install ONNX (GPU):

```shell
$ export DEST_DIR=$HOME/.local
$ wget https://github.com/microsoft/onnxruntime/releases/download/v1.20.1/onnxruntime-linux-x64-gpu-1.20.1.tgz
$ tar xf onnxruntime-linux-x64-gpu-1.20.1.tgz -C /tmp
$ pushd /tmp/onnxruntime-linux-x64-gpu-1.20.1
$ mkdir -p $DEST_DIR/{include/onnxruntime,lib}
$ cp -r include/* $DEST_DIR/include/onnxruntime
$ cp -r lib/* $DEST_DIR/lib
$ popd
$ rm -f onnxruntime-linux-x64-gpu-1.20.1.tgz
```

Install ONNX (CPU, mutually exclusive with GPU variant, see above):

```shell
$ export DEST_DIR=$HOME/.local
$ wget https://github.com/microsoft/onnxruntime/releases/download/v1.20.1/onnxruntime-linux-x64-1.20.1.tgz
$ tar xf onnxruntime-linux-x64-1.20.1.tgz -C /tmp
$ pushd /tmp/onnxruntime-linux-x64-1.20.1
$ mkdir -p $DEST_DIR/{include/onnxruntime,lib}
$ cp -r include/* $DEST_DIR/include/onnxruntime
$ cp -r lib/* $DEST_DIR/lib
$ popd
$ rm -f onnxruntime-linux-x64-1.20.1.tgz
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

Enable voice downloading:

```shell
export VOXER_VOICES_DOWNLOAD=1
export VOXER_VOICES_LIST="en_US+amy+medium;uk_UA+lada+x_low"
```

Disable voice downloading:

```shell
unset VOXER_VOICES_DOWNLOAD
unset VOXER_VOICES_LIST
```

### Locally

Build (without voices downloading):

```shell
# Debug
$ cmake --workflow --fresh --preset debug
# Release
$ cmake --workflow --fresh --preset release
```

Build and packing (without voices downloading):

```shell
# Debug
$ cmake --workflow --fresh --preset debug-and-pack
# Release
$ cmake --workflow --fresh --preset release-and-pack
```

### By Docker

Build:

```shell
# Debug
$ cmake --workflow --fresh --preset debug-docker
# Release
$ cmake --workflow --fresh --preset release-docker
```

Build and packing:

```shell
# Debug
$ cmake --workflow --fresh --preset debug-and-pack-docker
# Release
$ cmake --workflow --fresh --preset release-and-pack-docker
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
