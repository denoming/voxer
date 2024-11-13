# Voxer

## Introduction

**Voxer** is a tiny text to speech C++ library.

Available CMake options:

| Option                   | Default Value | Description                         |
|--------------------------|:-------------:|-------------------------------------|
| `VOXER_ENABLE_TESTS`     |      ON       | Enable unit tests                   |
| `VOXER_ENABLE_FORMATTED` |      ON       | Enable formatted audio data support |
| `VOXER_VOICES_DOWNLOAD`  |      OFF      | Enable voices downloading           |

## Configuration

To configure library you need to specify:

* path to `*.onnx and *.onnx.json` files
* path to `espeak-ng` files

Available environment variables:

| Variable          | Description                                           |
|-------------------|-------------------------------------------------------|
| `VOXER_MODEL_DIR` | The path to voice model file (*.onnx and *.onnx.json) |
| `VOXER_FILES_DIR` | The path to eSpeak-Ng data files                      |

## Building

Build artifacts default location is `<build-dir>/stage`.

The available packages:
 * [libvoxer_0.2.1_.deb](build-debug/libvoxer_0.2.1_.deb)
 * [libvoxer-dev_0.2.1_.deb](build-debug/libvoxer-dev_0.2.1_.deb)
 * (optional) [libvoxer-data_0.2.1_.deb](build-debug/libvoxer-data_0.2.1_.deb)

### Locally

Build debug artifacts:

```shell
$ cmake --workflow --fresh --preset debug
```

Build release artifacts:

```shell
$ cmake --workflow --fresh --preset release
```

Build and packing debug artifacts:

```shell
$ cmake --workflow --fresh --preset debug-and-pack
```

Build and packing release artifacts:

```shell
$ cmake --workflow --fresh --preset release-and-pack
```

### By Docker

Build debug artifacts:

```shell
$ cmake --workflow --fresh --preset debug-docker
```

Build release artifacts:

```shell
$ cmake --workflow --fresh --preset release-docker
```

Build and packing debug artifacts:

```shell
$ cmake --workflow --fresh --preset debug-and-pack-docker
```

Build and packing release artifacts:

```shell
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
 | voxer-cli --model $MODEL --files $FILES --wav --output-file -
 | gst-launch-1.0 -e fdsrc fd=0 ! wavparse ! audioconvert ! audioresample ! autoaudiosink sync=false
```

Play using gstreamer (RAW format):
```shell
$ echo $INPUT \
 | voxer-cli --model $MODEL --files $FILES --raw --output-file -
 | gst-launch-1.0 -e fdsrc fd=0 ! rawaudioparse num-channels=1 sample-rate=22050 ! audioconvert ! audioresample ! autoaudiosink sync=false
```
