# Cross Compiling for Raspberry PI 64-bit OS

## Setup cross compiler for Raspberry PI 64-bit OS

https://github.com/abhiTronix/raspberry-pi-cross-compilers/wiki/Cross-Compiler-CMake-Usage-Guide-with-rsynced-Raspberry-Pi-64-bit-OS#cross-compiler-cmake-usage-guide-with-rsynced-raspberry-pi-64-bit-os

## Build OpenCV

```sh
cd thirdparty/opencv
mkdir build && cd build

cmake -DCMAKE_TOOLCHAIN_FILE=<path/to/PI.cmake> -DBUILD_SHARED_LIBS=OFF ..

cmake --build . -j <number-process>
```

## Build library and example

```sh

mkdir build && cd build

OpenCV_DIR=</path/to/opencv/build> cmake -DCMAKE_TOOLCHAIN_FILE=<path/to/PI.cmake> ..

cmake --build . -j <number-process>

cmake --install . --prefix "<path/to/install/dir>"


```