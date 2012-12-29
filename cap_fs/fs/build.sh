#!/bin/bash
clang++ AppArmorCheck.cpp BaseNode.cpp OpenBaseNode.cpp BaseFs.cpp CapFs.cpp NoAccessFs.cpp  CapFsGuard.cpp main.cpp -I. -D_FILE_OFFSET_BITS=64 -lfuse -std=c++11 -Wno-deprecated-writable-strings
