#!/bin/bash 

mkdir -p output
cd output
cmake ..
make clean
make
cd ..
./output/LaserScanner $1
cd ..