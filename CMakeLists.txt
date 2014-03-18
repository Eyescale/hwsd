
# Copyright (c) 2011-2013 Stefan Eilemann <Stefan.Eilemann@epfl.ch>

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(hwsd)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
include(Buildyard)
if(BUILDYARD_STOP)
  return()
endif()

set(VERSION_MAJOR "1")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")

set(RELEASE_VERSION OFF) # OFF or ABI version
set(LAST_RELEASE 1.0.0)

include(Common)
include(GitTargets)
include(InstallSymlink)

if(APPLE)
  # WAR otherwise MacPorts X11 (/opt/local) is preferred
  list(INSERT CMAKE_PREFIX_PATH 0 /usr/X11R6)
endif()

include(FindPackages)
set(HWSD_DEPENDENT_LIBRARIES Lunchbox)

include_directories(BEFORE ${CMAKE_SOURCE_DIR}/libs)
add_subdirectory(apps)
add_subdirectory(hwsd)
add_subdirectory(doc)

include(DoxygenRule)
include(CPackConfig)

set(CTEST_CUSTOM_WARNING_EXCEPTION ${CTEST_CUSTOM_WARNING_EXCEPTION}
  # http://public.kitware.com/Bug/view.php?id=10179
  "ld: warning: duplicate dylib")
include(CTest)

include(PackageConfig)
