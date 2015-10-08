
# Copyright (c) 2011-2015 Stefan Eilemann <Stefan.Eilemann@epfl.ch>

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(hwsd)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common)

include(GitExternal)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "2")
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

common_package(Boost REQUIRED COMPONENTS program_options regex system)
common_package(Lunchbox REQUIRED)
common_package(OpenGL)
common_package(Qt5Core)
common_package(Qt5Network)
common_package(Servus REQUIRED)
common_package(X11)
common_package_post()

set(HWSD_DEPENDENT_LIBRARIES Lunchbox Servus)

add_subdirectory(apps)
add_subdirectory(hwsd)
add_subdirectory(doc)

set(DOXYGEN_MAINPAGE_MD ${PROJECT_SOURCE_DIR}/README.md)
set(DOXYGEN_EXTRA_INPUT ${DOXYGEN_MAINPAGE_MD})
include(CPackConfig)
include(DoxygenRule)
