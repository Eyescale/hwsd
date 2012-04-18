
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(gpusd)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "4")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(RELEASE_VERSION 100) # OFF or 'M00' ABI version

include(Common)
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/libs)
find_package(DNSSD)

if(APPLE)
  set(GPUSD_OSX_VERSION 10.6)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=${GPUSD_OSX_VERSION}" )

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

install(FILES CMake/FindGPUSD.cmake DESTINATION ${CMAKE_MODULE_INSTALL_PATH}
        COMPONENT dev)

include(CTest)
include(CPackConfig)
