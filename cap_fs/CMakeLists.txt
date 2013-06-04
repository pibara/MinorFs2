cmake_minimum_required(VERSION 2.8.7)
include(CheckIncludeFileCXX) 
#include(CheckFunctionExists)
include(CheckLibraryExists)
project(capfs CXX)
set(prefix      ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(libdir      ${CMAKE_INSTALL_PREFIX}/lib)
set(includedir  ${CMAKE_INSTALL_PREFIX}/include)
# Initialize CXXFLAGS.
SET(CMAKE_CXX_COMPILER             "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS                "-Wall -std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
SET (CMAKE_AR      "/usr/bin/llvm-ar")
SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
SET (CMAKE_NM      "/usr/bin/llvm-nm")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
ADD_DEFINITIONS(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE)
#Check for openssl lib and headers.
FIND_PACKAGE(OpenSSL REQUIRED)
#FIXME: The below commented out checks should get fixed so they actually work.
#CHECK_INCLUDE_FILE_CXX(openssl/hmac.h HAVE_HMAC_H)
#IF(HAVE_HMAC_H)
#ELSE(HAVE_HMAC_H)
#  MESSAGE(FATAL_ERROR "openssl/hmac.h headers not found.")
#ENDIF(HAVE_HMAC_H)
#Check for fuse library and headers.
#check_library_exists(fuse fuse_get_context "" HAVE_FUSE)
#IF(HAVE_FUSE)
#ELSE(HAVE_FUSE)
#  MESSAGE(FATAL_ERROR "No fuse library found.")
#ENDIF(HAVE_FUSE)
#CHECK_INCLUDE_FILE_CXX(fuse.h HAVE_FUSE_H)
#IF(HAVE_FUSE_H)
#ELSE(HAVE_FUSE_H)
#  MESSAGE(FATAL_ERROR "fuse.h headers not found.")
#ENDIF(HAVE_FUSE_H)
#Check for xattr header file
#CHECK_INCLUDE_FILE_CXX(attr/xattr.h HAVE_XATTR_H)
#IF(HAVE_XATTR_H)
#ELSE(HAVE_XATTR_H)
#  MESSAGE(FATAL_ERROR "attr/xattr.h headers not found.")
#ENDIF(HAVE_XATTR_H)
#The source files that make up capfs
set(capfs_SOURCES
        fs/BaseFs.cpp
        fs/BaseNode.cpp
        fs/CapFs.cpp
        fs/main.cpp
        fs/OpenBaseNode.cpp
        access/AppArmorCheck.cpp
        access/CapFsGuard.cpp 
        access/NoAccessFs.cpp
        algo/TripleHashLookup.cpp
        algo/TripleHashNode.cpp  
        algo/TripleHashParentChild.cpp)
add_executable(capfs ${capfs_SOURCES})
#We need to link against the fuse library and the openssl crypto library.
target_link_libraries(capfs fuse crypto)
install_targets(/sbin capfs)
