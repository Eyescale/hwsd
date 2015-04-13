
# Copyright (c) 2011-2015 Stefan Eilemann <Stefan.Eilemann@epfl.ch>

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(hwsd)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common)

include(GitExternal)
include(SubProject)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "1")
set(VERSION_PATCH "1")
set(VERSION_ABI 2)

include(Common)

set(DPUT_HOST "ppa:bbp/ppa") # Target ppa used by 'make dput'

set(COMMON_PROJECT_DOMAIN ch.eyescale)
set(HWSD_DESCRIPTION "Local and remote ZeroConf service discovery for hardware resources.")


if(APPLE)
  # WAR otherwise MacPorts X11 (/opt/local) is preferred
  list(INSERT CMAKE_PREFIX_PATH 0 /usr/X11R6)
endif()

include(FindPackages)
set(HWSD_DEPENDENT_LIBRARIES Lunchbox)

add_subdirectory(apps)
add_subdirectory(hwsd)
add_subdirectory(doc)

include(CPackConfig)
include(DoxygenRule)
