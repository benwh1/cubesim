#!/bin/bash
mkdir build
cd build
qmake ../cubesim.pro
make -j4
