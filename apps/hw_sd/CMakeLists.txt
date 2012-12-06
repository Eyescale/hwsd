
if(NOT LUNCHBOX_USE_DNSSD)
  return()
endif()

set(HEADERS)
set(SOURCES hw_sd.cpp)

include_directories(${LUNCHBOX_INCLUDE_DIRS})
set(LINK_LIBRARIES ${LUNCHBOX_LIBRARIES} hwsd)

if(APPLE)
  add_definitions(-DHWSD_GPU_CGL)
  list(APPEND LINK_LIBRARIES hwsd_gpu_cgl)
endif()
if(X11_FOUND)
  add_definitions(-DHWSD_GPU_GLX)
  list(APPEND LINK_LIBRARIES hwsd_gpu_glx)
endif()
if(WIN32)
  add_definitions(-DHWSD_GPU_WGL)
  list(APPEND LINK_LIBRARIES hwsd_gpu_wgl ws2_32)
endif()
if(HWLOC_FOUND)
  add_definitions(-DHWSD_NET_HWLOC)
  list(APPEND LINK_LIBRARIES hwsd_net_hwloc)
endif()

if(Boost_FOUND)
  list(APPEND LINK_LIBRARIES ${Boost_PROGRAM_OPTIONS_LIBRARY})
else()
  message(STATUS
    "boost::program_options missing, command line options not available")
endif()

source_group(\\ FILES ${SOURCES} ${HEADERS})
add_executable(hw_sd ${SOURCES} ${HEADERS})
target_link_libraries(hw_sd ${LINK_LIBRARIES})
install(TARGETS hw_sd DESTINATION bin COMPONENT daemon)

