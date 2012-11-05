
# Copyright (c) 2011-2012 Stefan Eilemann <Stefan.Eilemann@epfl.ch>

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(gpusd)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "5")
set(VERSION_PATCH "0")

set(RELEASE_VERSION OFF) # OFF or ABI version
set(LAST_RELEASE 1.4.0)

include(Common)
include(GitTargets)

if(APPLE)
  set(GPUSD_OSX_VERSION 10.6)
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -mmacosx-version-min=${GPUSD_OSX_VERSION}" )

  # WAR otherwise MacPorts X11 (/opt/local) is preferred
  list(INSERT CMAKE_PREFIX_PATH 0 /usr/X11R6)
endif()

include(FindPackages)

include_directories(BEFORE ${CMAKE_SOURCE_DIR}/libs)
add_subdirectory(apps)
add_subdirectory(libs)
add_subdirectory(doc)

install(FILES CMake/Findgpusd.cmake
  DESTINATION ${CMAKE_MODULE_INSTALL_PATH} COMPONENT dev)
install(FILES CMake/FindLibraryPackage.cmake
  DESTINATION ${CMAKE_MODULE_INSTALL_PATH}/Findgpusd COMPONENT dev)

include(DoxygenRule)
include(CTest)
include(CPackConfig)
