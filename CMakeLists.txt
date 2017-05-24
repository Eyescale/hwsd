
# Copyright (c) 2011-2017 Stefan Eilemann <Stefan.Eilemann@epfl.ch>

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(hwsd VERSION 2.0.1)
set(hwsd_VERSION_ABI 4)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
                              ${CMAKE_SOURCE_DIR}/CMake/common)
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/CMake/common/Common.cmake)
  message(FATAL_ERROR "CMake/common missing, run: git submodule update --init")
endif()

include(GitExternal)

set(HWSD_DEB_DEPENDS libx11-dev libgl1-mesa-dev qtbase5-dev
  libboost-program-options-dev libboost-regex-dev
  libboost-system-dev)
set(HWSD_PORT_DEPENDS boost qt5)

include(Common)

set(DPUT_HOST "ppa:bbp/ppa") # Target ppa used by 'make dput'

set(COMMON_PROJECT_DOMAIN ch.eyescale)
set(HWSD_DESCRIPTION "Local and remote ZeroConf service discovery for hardware resources")
set(HWSD_MAINTAINER "Stefan Eilemann <eile@eyescale.ch>")
set(HWSD_LICENSE LGPL)

if(APPLE)
  # WAR otherwise MacPorts X11 (/opt/local) is preferred
  list(INSERT CMAKE_PREFIX_PATH 0 /usr/X11R6 /usr/X11)
endif()

common_find_package(Boost REQUIRED COMPONENTS program_options regex system)
common_find_package(Lunchbox REQUIRED)
common_find_package(OpenGL)
common_find_package(Qt5Core)
common_find_package(Qt5Network)
common_find_package(Servus REQUIRED)
common_find_package(X11)
common_find_package_post()

set(HWSD_DEPENDENT_LIBRARIES Lunchbox Servus)

add_subdirectory(apps)
add_subdirectory(hwsd)
add_subdirectory(tests)
add_subdirectory(doc)

set(DOXYGEN_MAINPAGE_MD README.md)
set(DOXYGEN_EXTRA_INPUT ${PROJECT_SOURCE_DIR}/README.md)
include(DoxygenRule) # must be after all targets
include(CPackConfig)
