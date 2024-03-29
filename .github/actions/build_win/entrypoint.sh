#!/bin/sh

set -eux

export RACK_DIR=${GITHUB_WORKSPACE}/Rack-SDK
export RACK_USER_DIR=${GITHUB_WORKSPACE}

export CC=x86_64-w64-mingw32-gcc-posix
export CXX=x86_64-w64-mingw32-g++-posix
export STRIP=x86_64-w64-mingw32-strip

git submodule update --init --recursive

curl -L https://vcvrack.com/downloads/Rack-SDK-${RACK_SDK_VERSION}-win.zip -o rack-sdk.zip
unzip -o rack-sdk.zip
rm rack-sdk.zip

make clean
make dist
