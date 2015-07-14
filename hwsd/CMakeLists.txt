
source_group(\\ FILES CMakeLists.txt)

set(HWSD_PUBLIC_HEADERS # public, installed
  filter.h
  functions.h
  module.h
  module.ipp
  netInfo.h
  nodeInfo.h
  gpuInfo.h
  types.h
)

set(HWSD__HEADERS # private, not installed
  detail/dns_sd_module.h
)

set(HWSD_SOURCES
  filter.cpp
  functions.cpp
  netInfo.cpp
  nodeInfo.cpp
  gpuInfo.cpp
)

set(HWSD_LINK_LIBRARIES ${LUNCHBOX_LIBRARIES} ${Boost_REGEX_LIBRARY})

if(Qt5Network_FOUND)
  list(APPEND HWSD_LINK_LIBRARIES ${Qt5Network_LIBRARIES})
endif()

common_library(hwsd)
set_target_properties(hwsd PROPERTIES
  COMPILE_DEFINITIONS "HWSD_SHARED;HWSD_SHARED_INL")

add_definitions(-DHWSD_SHARED)
add_subdirectory(gpu)
add_subdirectory(net)
