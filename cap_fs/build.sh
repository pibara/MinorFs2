#!/bin/bash
clang++ access/AppArmorCheck.cpp fs/BaseNode.cpp fs/OpenBaseNode.cpp fs/BaseFs.cpp fs/CapFs.cpp access/NoAccessFs.cpp  access/CapFsGuard.cpp fs/main.cpp -I. -D_FILE_OFFSET_BITS=64 -lfuse -std=c++11 -Wno-deprecated-writable-strings
