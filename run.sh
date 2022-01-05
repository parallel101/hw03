#!/bin/sh
clear
set -e
cmake -B build
cmake --build build
build/main
