# Voxer

## Introduction

**Voxer** is a tiny text to speech C++ library.
Based on [piper-phonemize](https://github.com/rhasspy/piper-phonemize) neural text to speech system.

## Building

### By vcpkg (locally)

Debug configuration:
```shell
$ cmake --preset debug
$ cmake --build --preset build-debug
```
Release configuration:
```shell
$ cmake --preset release
$ cmake --build --preset build-release
```

## Installing

```shell
$ cmake --install <build-dir> --prefix <destination-path>/voxer
```

## Usage

To configure library you need to specify:
* path to `espeak-ng` files
* path to `*.onnx` model file

### By vcpkg package manager

* Add `voxer` dependency into project `vcpkg.json` file
* Download `*.onnx` and `*.onnx.json` model files (see [VOICES.md](VOICES.md) file)
