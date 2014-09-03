
# Copyright (c) 2011-2014 Stefan Eilemann <Stefan.Eilemann@epfl.ch>

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(hwsd)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
  ${PROJECT_SOURCE_DIR}/CMake/common)

include(GitExternal)
include(Buildyard)

if(BUILDYARD_STOP)
  return()
endif()

set(LAST_RELEASE 1.1.0)
set(VERSION_MAJOR "1")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION_ABI 2)

set(GITTARGETS_RELEASE_BRANCH minor)
set(DPUT_HOST "ppa:bbp/ppa") # Target ppa used by 'make dput'

set(HWSD_DESCRIPTION "Local and remote ZeroConf service discovery for hardware resources.")
set(COMMON_PROJECT_DOMAIN ch.eyescale)

include(Common)

if(APPLE)
  # WAR otherwise MacPorts X11 (/opt/local) is preferred
  list(INSERT CMAKE_PREFIX_PATH 0 /usr/X11R6)
endif()

include(FindPackages)
set(HWSD_DEPENDENT_LIBRARIES Lunchbox)

add_subdirectory(apps)
add_subdirectory(hwsd)
add_subdirectory(doc)

include(DoxygenRule)
include(CPackConfig)
include(PackageConfig)
include(GNUModules)

set(CTEST_CUSTOM_WARNING_EXCEPTION ${CTEST_CUSTOM_WARNING_EXCEPTION}
  # http://public.kitware.com/Bug/view.php?id=10179
  "ld: warning: duplicate dylib")
include(CTest)
