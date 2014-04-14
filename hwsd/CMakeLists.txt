
source_group(\\ FILES CMakeLists.txt)

set(HWSD_PUBLIC_HEADERS # public, installed
  ${COMMON_INCLUDES}
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
  ${COMMON_SOURCES}
  filter.cpp
  functions.cpp
  netInfo.cpp
  nodeInfo.cpp
  gpuInfo.cpp
)

set(HWSD_LINK_LIBRARIES ${LUNCHBOX_LIBRARIES} ${Boost_REGEX_LIBRARY})

if(QT_QTNETWORK_FOUND)
  include(${QT_USE_FILE})
  list(APPEND HWSD_LINK_LIBRARIES ${QT_LIBRARIES})
endif()

add_definitions(-DHWSD_SHARED -DHWSD_SHARED_INL)
common_library(hwsd)

add_subdirectory(gpu)
add_subdirectory(net)
