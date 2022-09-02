#!/bin/sh
set -xe
g++ -O0 -Wall -Wextra -pedantic -DNO_ASSERTIONS_ -o examples examples.cpp
./examples