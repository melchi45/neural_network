# neural_network

This application for Neural Network Example class for machine learning 
at Aug. 2001 by Youngho Kim from Chonnam National University, South Korean.

I was coded classes based MLP Neural Network algorithm from Academic Press, Inc.
But, you can access original code to this url:
http://www.realtechsupport.org/UB/SR/neuralnets/code%20examples/C++/?C=M;O=A

If you want study Artificial Neural Network, you can study this documents from
http://www.realtechsupport.org/UB/SR/neuralnets/NNpapers/

## Generate build project using cmake

Actually, This command makes the build project script file for Visual Studio, Linux/Unix Makefiles, Xcode project.

### For Windows
#### For 32 bit Windows from dos prompt
```shell
# mkdir build
# cd build
# cmake .. -G "Visual Studio 15 2017"
```

#### For 64 bit Windows from dos prompt
```shell
# mkdir build
# cd build
# cmake .. -G "Visual Studio 15 2017 Win64"
```

#### For ARM from from dos prompt
```shell
# mkdir build
# cd build
# cmake .. -G "Visual Studio 15 2017 ARM"
```

### For Xcode
```shell
# mkdir build
# cd build
# cmake .. -G "XCode" \
  -DCMAKE_VERBOSE_MAKEFILE=ON \
```

### For Linux
```shell
# mkdir build
# cd build
# export OUT_PATH=./install
# cmake .. -G "Unix Makefiles" \
  -DCMAKE_VERBOSE_MAKEFILE=ON \
  -DCMAKE_BUILD_TYPE=release \
  -DCMAKE_INSTALL_PREFIX=${OUT_PATH}
# make; make install  
```

For Linux for arm with toolchain
{TOOLCHAIN_PATH} is toolchain path for ARM CPU from manufacture. 

For example, If you use the Raspberry PI 3 Model B arm board,
If you installed the toolchain by referring to https://goo.gl/TtcjGb, you are installed toolchain path is pri.

ref: http://www.hertaville.com/development-environment-raspberry-pi-cross-compiler.html.

TOOLCHAIN_PATH is ~/pri/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin.

```shell
# mkdir build
# cd build
# export OUT_PATH=./install
# export CROSS_COMPILE=${TOOLCHAIN_PATH}/arm-linux-gnueabihf-
# cmake .. -G "Unix Makefiles" \
  -DCMAKE_VERBOSE_MAKEFILE=ON \
  -DCMAKE_BUILD_TYPE=release \
  -DCMAKE_C_COMPILER=${CROSS_COMPILE}gcc \
  -DCMAKE_CXX_COMPILER=${CROSS_COMPILE}g++ \
  -DCMAKE_INSTALL_PREFIX=${OUT_PATH} \
  -DCMAKE_STRIP=${CROSS_COMPILE}strip \
  -DCMAKE_AR=${CROSS_COMPILE}ar \
  -DCMAKE_LD=${CROSS_COMPILE}ld \
  -DCMAKE_RANLIB=${CROSS_COMPILE}ranlib  \
  -DCMAKE_NM=${CROSS_COMPILE}nm \
  -DCMAKE_OBJCOPY=${CROSS_COMPILE}objcopy \
  -DCMAKE_OBJDUMP=${CROSS_COMPILE}objdump \
  -DCMAKE_LINKER=${CROSS_COMPILE}ld
# make; make install  
```

## Build using cmake file without Visual Studio IDE

You want to build without Visual Studio IDE or You want to build shared or static mode.

```shell
cmake . -Bshared -G "Visual Studio 15 2017" -DCMAKE_VERBOSE_MAKEFILE=ON
cmake --build shared --config Release
```

Or 

```shell
cmake . -Bstatic -G "Visual Studio 15 2017" -DCMAKE_VERBOSE_MAKEFILE=ON
cmake --build static --config Release

## Example
https://github.com/melchi45/NN_Analysis
https://github.com/melchi45/SOM_Cluster
