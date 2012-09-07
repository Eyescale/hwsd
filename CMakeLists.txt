
# Copyright (c) 2011-2012 Stefan Eilemann <Stefan.Eilemann@epfl.ch>

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(gpusd)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "4")
set(VERSION_PATCH "1")

set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(RELEASE_VERSION 1) # OFF or ABI version

include(Common)
include(GitTargets)
include(DoxygenSetup)
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/libs)

find_package(Boost COMPONENTS program_options) 
find_package(Lunchbox 1.3.5)

if(APPLE)
  set(GPUSD_OSX_VERSION 10.6)
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -mmacosx-version-min=${GPUSD_OSX_VERSION}" )

  # WAR otherwise MacPorts X11 (/opt/local) is preferred
  LIST(REMOVE_ITEM CMAKE_SYSTEM_LIBRARY_PATH /opt/local/lib)
  LIST(REMOVE_ITEM CMAKE_SYSTEM_PREFIX_PATH /opt/local)
endif()

find_package(X11)

if(APPLE)
  LIST(APPEND CMAKE_SYSTEM_LIBRARY_PATH /opt/local/lib)
  LIST(APPEND CMAKE_SYSTEM_PREFIX_PATH /opt/local)
endif(APPLE)

add_subdirectory(apps)
add_subdirectory(libs)
add_subdirectory(doc)
include(DoxygenRule) # must be after all targets

install(FILES CMake/Findgpusd.cmake
  DESTINATION ${CMAKE_MODULE_INSTALL_PATH} COMPONENT dev)
install(FILES CMake/FindLibraryPackage.cmake
  DESTINATION ${CMAKE_MODULE_INSTALL_PATH}/Findgpusd COMPONENT dev)

include(CTest)
include(CPackConfig)
