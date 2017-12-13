#!/bin/bash

CFLAGS="-ggdb3 -O0" CXXFLAGS="-ggdb3 -O0" LDFLAGS="-ggdb3" ./configure --with-w32
make -j8
