# Tensorflow Lite Object Detection Library

## Content
- [Introduction](#introduction)
- [Build Steps](#build-steps)
    - [Raspberry PI Cross Compiling](#raspberry-pi-cross-compiling)

## Introduction

This is a C++ library for tensorflow lite object detecion models. 

The base <b>ObjectDetector</b> class is developed to use with [ssd_mobilenet_v1 model](https://www.tensorflow.org/lite/examples/object_detection/overview).

To customize for other model, user need to derive ObjectDetector and rewrite PreProcess and PostProcess functions to align with the model inputs and outputs.

This library also has the configuration for setting up the model or labels path, and other parameter.

For usage, see the minimal example [examples/minimal](examples/minimal/main.cpp).

## Build Steps

This library uses CMake to configurate and build. So you need to install CMake tool first.

Then clone the repository:

```sh
git clone --recursive https://github.com/bienxanh1901/tflite-object-detection
```

### Raspberry PI Cross Compiling

See [cross-build-raspberry-PI.md](doc/cross-compile-raspi-64-bit.md)