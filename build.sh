#!/bin/bash
mkdir build
cd build
qmake ../src/cubesim.pro
make -j4
